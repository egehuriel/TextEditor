#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "../include/editor.h"
#include "../include/garbageCollection.h"
//#include "../include/ui.h"
//#include "../include/operations.h" 

//operation ve ui oncesi placeholder
void insert(int index){}
void delete(int index){}
void replace(int index){}
void print_buffer(void){}
int cursorLine(void){return 0;}
int cursorChar(void){return 0;}

int main(void){
    int ch;
    char filename[256];
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);
    while(true){
        ch = getch();
        switch(ch){
            //echo file
            case 'E':
            case 'e':
                echo();
                mvprintw(0,0, "file name: ");
                noecho();
                edit(filename);
                print_buffer();
                break;
            //insert
            case 'I':
            case 'i':
                insert(cursorLine());
                maybeAutoGC();
                break;
            //delete
            case 'D':
            case 'd':
                delete(cursorLine());
            //replace
            case 'R':
            case 'r':
                replace(cursorChar());
            //print
            case 'P':
            case 'p':
                print_buffer();
                break;
            //save
            case 'S':
            case 's':
                save();
                break;
            //garbage collection -> garbagecollection.c
            case 'G':
            case 'g':
                garbageCollection();
                print_buffer();
                break;
            //quit -> exit (normal terminal)
            case 'Q':
            case 'q':
                endwin();
                return 0;
            //ui keyup -> bir satir yukari
            case KEY_UP:
                //cursor line ui.c
                break;
            // ui keywdown -> bir satir asagi
            case KEY_DOWN:
                //cursor line ui.c
                break;
            //ui keyleft -> bir karakter sola
            case KEY_LEFT:
                //cursor char ui.c
                break;
            //ui keyright -> bir karakter saga
            case KEY_RIGHT:
                //cursor char ui.c
                break;
            case '\n':
            //keyenter -> satir secimi
            case KEY_ENTER:
                //cursor char ui.c
                break;
            
            }
    }
}