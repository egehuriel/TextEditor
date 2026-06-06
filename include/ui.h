#ifndef UI_H
#define UI_H

#include <ncurses.h>

/*
 * ui.h  —  Kişi 3: ncurses UI & Cursor
 * ─────────────────────────────────────
 * ui_init()     : ncurses kurulumu (initscr, keypad, raw, noecho)
 * print_buffer(): textbuffer'ı head'den tail'e ekrana basar
 * cursorLine()  : ekrandaki Y → textbuffer indeksi
 * cursorChar()  : ekrandaki X → satır içi karakter pozisyonu
 * cursorLineNth(): ekrandaki Y → linked-list sıra numarası 
 */

#define UI_TEXT_START_ROW  2
#define COLOR_SELECTED     1

void ui_init(void);
void ui_cleanup(void);

void print_buffer(void);

int cursorLine(void);
int cursorChar(void);

/* insert/delete/replace nthNode() kullandığı için sıra numarası döner */
int cursorLineNth(void);

void ui_move_up(void);
void ui_move_down(void);
void ui_move_left(void);
void ui_move_right(void);

void ui_status(const char *msg);

#endif 
