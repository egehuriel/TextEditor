/*
 * operations.c  —  Kişi 2: Düzenleme İşlemleri
 * ─────────────────────────────────────────────
 * insert(index)   : `index` indeksli satırın ÖNÜNE yeni satır ekle
 * deleteNode(int) : satırı linked-list'ten kopar (fiziksel silme GC'ye bırakılır)
 * replace(index, col) : belirtilen satır+sütundaki tek karakteri değiştir
 */

//#include <stdio.h>
#include <string.h>
#include "../include/editor.h"
#include "../include/operations.h"
#include <ncurses.h>

/* ─────────────────────────────────────────────────────────────
 * Yardımcı: textbuffer[]'da gerçekten linked-list üyesi olan,
 *           "index" sıra numarasına (0-tabanlı) sahip node'u
 *           döndürür.  index == NIL → tail'in index+1'i demek değil,
 *           buraya gelinmemeli; caller dikkatli olmalı.
 * ────────────────────────────────────────────────────────────── */
static int nthNode(int n) {
    int cur = head;
    int i   = 0;
    while (cur != NIL && i < n) {
        cur = textbuffer[cur].next;
        i++;
    }
    return cur;   /* NIL ise liste n'den kısa demektir */
}

/* ─────────────────────────────────────────────────────────────
 * insert(index)
 *
 * "index" sıra numarasına (0-tabanlı) sahip satırın ÖNÜNE yeni
 * bir satır ekler.  index == 0  → listenin başına eklenir.
 * index < 0 veya index >= liste uzunluğu  → listenin SONUNA eklenir.
 *
 * Fiziksel yer   : textbuffer[freeIndex]
 * Bağlantılar    : yeni node'un prev/next + komşuların next/prev
 *                  güncellenir.
 * Dönüş değeri   : yeni node'un textbuffer indeksi; hata: -1
 * ────────────────────────────────────────────────────────────── */
int insert(int index) {
    char  newLine[MAX_CHARS];
    int   newIdx;
    int   afterNode;   /* yeni node'un ÖNÜNE eklendiği mevcut node */
    int   prevNode;

    /* Buffer dolu mu? */
    if (freeIndex >= MAX_LINES) {
        // printf("HATA: Textbuffer dolu, insert() basarisiz.\n");
        return -1;
    }

    /* Kullanıcıdan yeni satır içeriğini al */
    //ncurses implemeantion --> no printf 
    mvprintw(LINES - 1, 0, "NEW LINE: ");
    clrtoeol();
    echo();
    getnstr(newLine, MAX_CHARS -1);
    noecho();

    /* Yeni node'un fiziksel konumu */
    newIdx = freeIndex;
    freeIndex++;

    /* İçeriği kopyala */
    strncpy(textbuffer[newIdx].statement, newLine, MAX_CHARS - 1);
    textbuffer[newIdx].statement[MAX_CHARS - 1] = '\0';

    /* Liste boşsa direkt ilk eleman yap */
    if (head == NIL) {
        textbuffer[newIdx].prev = NIL;
        textbuffer[newIdx].next = NIL;
        head = newIdx;
        tail = newIdx;
        return newIdx;
    }

    // index NIL ise sona ekle
    if (index == NIL){
        textbuffer[newIdx].prev = tail;
        textbuffer[newIdx].next = NIL;
        textbuffer[tail].next = newIdx;
        tail = newIdx;
        return newIdx;
    }

    /* Hedef sıradaki mevcut node'u bul */
    afterNode = nthNode(index);

    if (afterNode == NIL) {
        /*
         * index liste uzunluğundan büyük → sona ekle
         * tail'in arkasına bağla
         */
        prevNode = tail;
        textbuffer[newIdx].prev = prevNode;
        textbuffer[newIdx].next = NIL;
        textbuffer[prevNode].next = newIdx;
        tail = newIdx;
    } else if (afterNode == head && index == 0) {
        /* Listenin başına ekle */
        textbuffer[newIdx].prev = NIL;
        textbuffer[newIdx].next = head;
        textbuffer[head].prev   = newIdx;
        head = newIdx;
    } else {
        /* Ortaya ekle: afterNode'un hemen ÖNÜNe */
        prevNode = textbuffer[afterNode].prev;

        textbuffer[newIdx].prev = prevNode;
        textbuffer[newIdx].next = afterNode;

        if (prevNode != NIL) {
            textbuffer[prevNode].next = newIdx;
        }
        textbuffer[afterNode].prev = newIdx;

        /* Eğer afterNode head idiyse (bu dal tutmaz ama güvenlik) */
        if (afterNode == head) {
            head = newIdx;
        }
    }

    return newIdx;
}

/* ─────────────────────────────────────────────────────────────
 * deleteNode(index)
 *
 * "index" sıra numarasına sahip node'u linked-list'ten koparır.
 * Node'un textbuffer slotu serbest bırakılmaz; bunu GC yapar.
 *
 * Dönüş değeri : 0 başarı, -1 geçersiz index
 * ────────────────────────────────────────────────────────────── */
int deleteNode(int index) {
    //int target;
    int prevNode;
    int nextNode;

    if (head == NIL || index == NIL) {
        //printf("HATA: Liste bos, silinemez.\n");
        return -1;
    }

    prevNode = textbuffer[index].prev;
    nextNode = textbuffer[index].next;

    /* Komşuların bağlarını güncelle */
    if (prevNode != NIL) {
        textbuffer[prevNode].next = nextNode;
    } else {
        /* Silinen node head'di */
        head = nextNode;
    }

    if (nextNode != NIL) {
        textbuffer[nextNode].prev = prevNode;
    } else {
        /* Silinen node tail'di */
        tail = prevNode;
    }

    /*
     * Node'un kendi bağlarını NIL yap – GC'ye işaret.
     * (İçerik silinmiyor; GC sıkıştıracak.)
     */
    textbuffer[index].prev = NIL;
    textbuffer[index].next = NIL;

    return 0;
}

/* ─────────────────────────────────────────────────────────────
 * replace(index, col)
 *
 * "index" sıra numarasındaki satırın "col" sütunundaki (0-tabanlı)
 * TEK karakteri yeni bir karakterle değiştirir.
 *
 * Dönüş değeri : 0 başarı, -1 hata
 * ────────────────────────────────────────────────────────────── */
int replace(int index, int col) {
    //int   target;
    int   len;
    char  newChar;
    char  oldChar;

    if (head == NIL) {
        //printf("HATA: Liste bos.\n");
        return -1;
    }

    len = (int)strlen(textbuffer[index].statement);

    if (col < 0 || col >= len) {
        //printf("HATA: Gecersiz sutun %d (satir uzunlugu: %d).\n", col, len);
        return -1;
    }

    /* Kullanıcıdan yeni karakter al */
    //printf("Yeni karakter: ");
    mvprintw(LINES -1, 0, "NEW CHAR: ");
    clrtoeol();
    refresh();
    noecho();
    newChar = (char)getch();
    textbuffer[index].statement[col] = newChar;

    //printf("Satir %d, Sutun %d: '%c' -> '%c'\n", index, col, oldChar, newChar);
    //printf("Guncellenmis satir: %s\n", textbuffer[target].statement);

    return 0;
}
