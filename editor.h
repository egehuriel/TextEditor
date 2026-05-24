#ifndef EDITOR_H
#define EDITOR_H

#define MAX_LINES 100  // dosyadaki maksimum satır sayısı
#define MAX_CHARS 40   // bir satırdaki maksimum karakter sayısı
#define NIL -1         // bağlantı yok anlamına gelir

// textbuffer içindeki her satır bir node
typedef struct node {
    char statement[MAX_CHARS]; // Satırın içeriği
    int next;                  // Sonraki satırın indeksi
    int prev;                  // Önceki satırın indeksi
} Node;

// Global değişkenler
extern Node textbuffer[MAX_LINES];
extern int head;
extern int tail;
extern int freeIndex;
extern char currentFilename[256];

// Fonksiyon prototipleri
void clearBuffer(void);
void stripNewline(char line[]);
void edit(char *filename);
void save(void);

#endif