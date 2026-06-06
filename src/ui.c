#include <string.h>
#include <ncurses.h>
#include "../include/editor.h"
#include "../include/ui.h"

#define MAX_DISPLAY_LINES 30

static int colorsReady = 0;

static void initColors(void) {
    if (!colorsReady && has_colors()) {
        start_color();
        init_pair(COLOR_SELECTED, COLOR_BLACK, COLOR_GREEN);
        colorsReady = 1;
    }
}

void ui_init(void) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
}

void ui_cleanup(void) {
    endwin();
}

void print_buffer(void) {
    int current;
    int row;
    int screenRows, screenCols;
    int curY, curX;

    initColors();
    getmaxyx(stdscr, screenRows, screenCols);
    getyx(stdscr, curY, curX);

    for (row = UI_TEXT_START_ROW; row < screenRows - 1; row++) {
        move(row, 0);
        clrtoeol();
    }

    move(0, 0);
    clrtoeol();
    attron(A_BOLD);
    mvprintw(0, 0,
        " CSE232 Text Editor  |  E)dit  I)nsert  D)elete  R)eplace  P)rint  S)ave  G)C  Q)uit");
    attroff(A_BOLD);

    move(1, 0);
    clrtoeol();
    for (int c = 0; c < screenCols; c++) {
        mvaddch(1, c, '-');
    }

    current = head;
    row = UI_TEXT_START_ROW;

    while (current != NIL && row < UI_TEXT_START_ROW + MAX_DISPLAY_LINES) {
        int isSelected = (row == curY);

        if (isSelected) {
            attron(COLOR_PAIR(COLOR_SELECTED) | A_BOLD);
        }

        mvprintw(row, 0, "%-*s", screenCols - 1, textbuffer[current].statement);

        if (isSelected) {
            attroff(COLOR_PAIR(COLOR_SELECTED) | A_BOLD);
        }

        current = textbuffer[current].next;
        row++;
    }

    {
        int clampY = curY;
        int clampX = curX;
        int lastRow = row - 1;

        if (clampY < UI_TEXT_START_ROW) {
            clampY = UI_TEXT_START_ROW;
        }
        if (lastRow >= UI_TEXT_START_ROW && clampY > lastRow) {
            clampY = lastRow;
        }
        if (clampX < 0) {
            clampX = 0;
        }
        if (clampX >= screenCols) {
            clampX = screenCols - 1;
        }

        move(clampY, clampX);
    }

    refresh();
}

int cursorLineNth(void) {
    int curY, curX;
    int visualRow;

    getyx(stdscr, curY, curX);
    visualRow = curY - UI_TEXT_START_ROW;

    if (visualRow < 0) {
        return 0;
    }

    return visualRow;
}

int cursorLine(void) {
    int visualRow = cursorLineNth();
    int current = head;
    int i;

    if (visualRow < 0) {
        return NIL;
    }

    for (i = 0; i < visualRow; i++) {
        if (current == NIL) {
            return NIL;
        }
        current = textbuffer[current].next;
    }

    return current;
}

int cursorChar(void) {
    int curY, curX;
    int bufIdx;
    int lineLen;

    getyx(stdscr, curY, curX);
    bufIdx = cursorLine();

    if (bufIdx == NIL) {
        return 0;
    }

    lineLen = (int)strlen(textbuffer[bufIdx].statement);

    if (lineLen == 0) {
        return 0;
    }

    if (curX >= lineLen) {
        curX = lineLen - 1;
    }
    if (curX < 0) {
        curX = 0;
    }

    return curX;
}

void ui_move_up(void) {
    int curY, curX;

    getyx(stdscr, curY, curX);

    if (curY > UI_TEXT_START_ROW) {
        move(curY - 1, curX);
        print_buffer();
    }
}

void ui_move_down(void) {
    int curY, curX;
    int lastRow;
    int current;
    int count;

    getyx(stdscr, curY, curX);

    current = head;
    count = 0;
    while (current != NIL && count < MAX_DISPLAY_LINES) {
        current = textbuffer[current].next;
        count++;
    }

    if (count == 0) {
        return;
    }

    lastRow = UI_TEXT_START_ROW + count - 1;

    if (curY < lastRow) {
        move(curY + 1, curX);
        print_buffer();
    }
}

void ui_move_left(void) {
    int curY, curX;

    getyx(stdscr, curY, curX);

    if (curX > 0) {
        move(curY, curX - 1);
        refresh();
    }
}

void ui_move_right(void) {
    int curY, curX;
    int bufIdx;
    int lineLen;

    getyx(stdscr, curY, curX);
    bufIdx = cursorLine();

    if (bufIdx == NIL) {
        return;
    }

    lineLen = (int)strlen(textbuffer[bufIdx].statement);

    if (lineLen > 0 && curX < lineLen - 1) {
        move(curY, curX + 1);
        refresh();
    }
}

void ui_status(const char *msg) {
    int screenRows, screenCols;

    getmaxyx(stdscr, screenRows, screenCols);

    move(screenRows - 1, 0);
    clrtoeol();
    attron(A_REVERSE);
    mvprintw(screenRows - 1, 0, " %s", msg);
    attroff(A_REVERSE);
    refresh();
}
