/*
main.c - Ege Huriel (Kisi 5)
Integration
- ana komut dongusu
- tus navigasyonu / keymapping
*/
#include <stdio.h>
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
            //edit -> dosya yukeleme | secme
            case 'E':
            case 'e':
                int sRows, sCols;
                getmaxyx(stdscr, sRows, sCols);
                move(sRows - 1, 0);
                clrtoeol();
                mvprintw(sRows - 1, 0, "file name: ");
                refresh();
                echo();
                getnstr(filename, sizeof(filename) - 1);
                noecho();
                edit(filename);
                print_buffer();
                ui_status("File loaded");
                move(UI_TEXT_START_ROW, 0);
                refresh();
                break;
            //insert
            case 'I':
            case 'i':
                insert(cursorLine());
                maybeAutoGC();
                print_buffer();
                ui_status("Line inserted.");
                break;
            //delete                
            case 'D':
            case 'd':
                if(deleteNode(cursorLine()) == 0){
                    maybeAutoGC();
                    print_buffer();
                    ui_status("Line deleted");
                } else {
                    ui_status("Delete failed: invalid line.");
                }
                break;
            //replace
            case 'R':
            case 'r':
                if(replace(cursorLine(), cursorChar()) == 0){
                    print_buffer();
                    ui_status("Character replaced.");
                } else {
                    ui_status("Replace failed: invalid line or column.");
                }
                break;
            //print
            case 'P':
            case 'p':
                print_buffer();
                ui_status("Text displayed.");
                break;
            //save
            case 'S':
            case 's':
                save();
                ui_status("File saved.");
                break;
            //garbage collection
            case 'G':
            case 'g': {
                int remaining = garbageCollection();
                print_buffer();
                char msg[64];
                snprintf(msg, sizeof(msg), "GC complete. Active lines: %d", remaining);
                ui_status(msg);
                break;
            }
            //quit
            case 'Q':
            case 'q':
                ui_cleanup();
                return 0;
            //bir karakter yukari
            case KEY_UP:
                ui_move_up();
                break;
            //bir karakter asagi
            case KEY_DOWN:
                ui_move_down();
                break;
            //bir karakter sola
            case KEY_LEFT:
                ui_move_left();
                break;
            //bir karakter saga
            case KEY_RIGHT:
                ui_move_right();
                break;
            //satir secimi
            case '\n':
            case KEY_ENTER:
                print_buffer();
                break;

            default:
                break;
        }
    }
}
