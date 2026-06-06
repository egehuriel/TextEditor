/*
main.c - Ege Huriel (Kisi 5)
Integration
- ana komut dongusu
- tus navigasyonu / keymapping
*/
#include <string.h>
#include <ncurses.h>
#include "../include/editor.h"
#include "../include/garbageCollection.h"
#include "../include/operations.h"
#include "../include/ui.h"

int main(void) {
    int ch;
    char filename[256];
    
    //initscr();
    //raw();
    //noecho();
    //keypad(stdscr, TRUE);
    
    //ncurses kurulm
    ui_init();
    print_buffer();
    ui_status("Command: E)dit  I)nsert  D)elete  R)eplace  P)rint  S)ave  G)C  Q)uit");
    while(true){
        ch = getch();

        switch (ch) {

            case 'E':
            case 'e':
                ui_status("Filename: ");
                echo();
                mvprintw(0,0, "file name: ");
                scanw("%s", filename);
                noecho();
                edit(filename);
                print_buffer();
                move(UI_TEXT_START_ROW, 0);
                refresh();
                break;

            case 'I':
            case 'i':
                insert(cursorLineNth());
                maybeAutoGC();
                print_buffer();
                ui_status("Line inserted.");
                break;

            case 'D':
            case 'd':
                deleteNode(cursorLine());
                maybeAutoGC();
                break;
            //replace
            case 'R':
            case 'r':
                replace(cursorLine(), cursorChar());
                break;
            //print
            case 'P':
            case 'p':
                print_buffer();
                ui_status("Text displayed.");
                break;

            case 'S':
            case 's':
                save();
                ui_status("File saved.");
                break;

            case 'G':
            case 'g': {
                int remaining = garbageCollection();
                print_buffer();
                char msg[64];
                snprintf(msg, sizeof(msg), "GC complete. Active lines: %d", remaining);
                ui_status(msg);
                break;
            }

            case 'Q':
            case 'q':
                ui_cleanup();
                return 0;

            case KEY_UP:
                ui_move_up();
                break;

            case KEY_DOWN:
                ui_move_down();
                break;

            case KEY_LEFT:
                ui_move_left();
                break;

            case KEY_RIGHT:
                ui_move_right();
                break;

            case '\n':
            case KEY_ENTER:
                print_buffer();
                break;

            default:
                break;
        }
    }
}
