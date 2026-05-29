#include <stdio.h>
#include <string.h>
#include "editor.h"

// Bütün satırların dizisi
Node textbuffer[MAX_LINES];

// Bağlı listenin başlangıcı
int head = NIL;

// Bağlı listenin sonu
int tail = NIL;

// textbuffer[] içindeki ilk boş indeks
int freeIndex = 0;

// Açılan dosyanın ismi, save() bunu kullanır
char currentFilename[256];

// Buffer'ı tamamen sıfırlar
void clearBuffer(void) {
    int i;

    for (i = 0; i < MAX_LINES; i++) {
        textbuffer[i].statement[0] = '\0'; // satırın içeriğini null yapar
        textbuffer[i].next = NIL;
        textbuffer[i].prev = NIL;
    }

    head = NIL;
    tail = NIL;
    freeIndex = 0;
}

// Satır sonundaki '\n' veya '\r' karakterini siler
void stripNewline(char line[]) {
    int i = 0;

    while (line[i] != '\0') {
        if (line[i] == '\n' || line[i] == '\r') {
            line[i] = '\0';
            break;
        }
        i++;
    }
}

// Dosyayı açar, satırları textbuffer içine yükler ve bağlantıları kurar
void edit(char *filename) {
    FILE *file;
    char line[200];  // satırda kolay değişiklik yapmak için karakter sayısı yüksek
    int i = 0;

    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Dosya açılamadı: %s\n", filename);
        return;
    }

    clearBuffer();   // önceden buffer'da kalan veri varsa silinir

    // Dosya adını currentFilename içinde sonrasında save() sırasında kullanılmak üzere saklar
    // Son karakteri '\0' yaparak string'in düzgün bitmesini garanti eder
    strncpy(currentFilename, filename, sizeof(currentFilename) - 1);
    currentFilename[sizeof(currentFilename) - 1] = '\0';

    while (fgets(line, sizeof(line), file) != NULL && i < MAX_LINES) {
        stripNewline(line);

        // Satır içeriğini node'un satırına kopyala
        strncpy(textbuffer[i].statement, line, MAX_CHARS - 1);
        textbuffer[i].statement[MAX_CHARS - 1] = '\0';

        // Önceki node indeksi
        if (i == 0) {
            textbuffer[i].prev = NIL;
        } else {
            textbuffer[i].prev = i - 1;
        }

        // Sonraki node yokmuş gibi varsay
        textbuffer[i].next = NIL;

        // Bir önceki node varsa, onun next değerini bu node'a bağla
        if (i > 0) {
            textbuffer[i - 1].next = i;
        }

        i++;
    }

    fclose(file);

    if (i == 0) {
        // Dosya boş
        head = NIL;
        tail = NIL;
        freeIndex = 0;
    } else {
        // İlk satır 0. indekste, son satır i - 1. indekste
        head = 0;
        tail = i - 1;
        freeIndex = i; // i MAX_LINES'a eşitse boş satır kalmamış demektir
    }

    if (i >= MAX_LINES) {
        printf("Buffer doldu. En fazla 100 satır yüklendi.\n");
    }
}


// textbuffer içindeki geçerli satırları dosyaya yazar
void save(void) {
    FILE *file;
    int current;

    if (currentFilename[0] == '\0') {
        printf("Kaydedilecek dosya yok. Önce edit() ile dosya açılmalı.\n");
        return;
    }

    file = fopen(currentFilename, "w");

    if (file == NULL) {
        printf("Dosya kaydedilemedi: %s\n", currentFilename);
        return;
    }

    current = head;

    // head değerinden başlayıp next bağlantılarına göre sırayla satırları yazar
    while (current != NIL) {
        fprintf(file, "%s\n", textbuffer[current].statement);
        current = textbuffer[current].next;
    }

    fclose(file);
}