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

    ui_init();
    ui_status("Ready. Press E to open a file.");

    while (1) {
        ch = getch();

        switch (ch) {

            case 'E':
            case 'e':
                ui_status("Filename: ");
                echo();
                getnstr(filename, sizeof(filename) - 1);
                noecho();
                edit(filename);
                print_buffer();
                ui_status("File loaded.");
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
                if (deleteNode(cursorLineNth()) == 0) {
                    maybeAutoGC();
                    print_buffer();
                    ui_status("Line deleted.");
                } else {
                    ui_status("ERROR: Delete failed.");
                }
                break;

            case 'R':
            case 'r':
                if (replace(cursorLineNth(), cursorChar()) == 0) {
                    print_buffer();
                    ui_status("Character replaced.");
                } else {
                    ui_status("ERROR: Replace failed.");
                }
                break;

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
