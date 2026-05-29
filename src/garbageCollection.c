#include <string.h>
#include "editor.h"

// Otomatik GC tetiklemesi için sayaç
int gcCounter = 0;

int garbageCollection(void) {
    Node temp[MAX_LINES]; 
    int count = 0;          //geçerli satır sayısı
    int current = head;
    int i;

    //geçerli satırları topluyor
    while (current != NIL && count < MAX_LINES) {
       strncpy(temp[count].statement,
                textbuffer[current].statement,
                MAX_CHARS);
        temp[count].statement[MAX_CHARS - 1] = '\0';
        count++;
        current = textbuffer[current].next;
    }

    clearBuffer();  

    // Tempteki satırları yerleştiriyorum
    for (i = 0; i < count; i++) {
        strncpy(textbuffer[i].statement, temp[i].statement, MAX_CHARS);
        textbuffer[i].statement[MAX_CHARS - 1] = '\0';

        textbuffer[i].prev = (i == 0)         ? NIL : i - 1;
        textbuffer[i].next = (i == count - 1) ? NIL : i + 1;
    }

    if (count == 0) {
        head = NIL;
        tail = NIL;
        freeIndex = 0;
    } else {
        head = 0;
        tail = count - 1;
        freeIndex = count;
    }

    gcCounter = 0;
    return count;
}

void maybeAutoGC(void) {
    gcCounter++;
    if (gcCounter >= 10 || freeIndex >= MAX_LINES) {
        garbageCollection();
    }
}