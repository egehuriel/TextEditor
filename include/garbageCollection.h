#ifndef GC_H
#define GC_H

// Geçerli satırları textbuffer'ın başına sıkıştırır,
// silinmiş slot'ları sona toplar.
// Geri dönüş: kalan geçerli satır sayısı.
int garbageCollection(void);

// insert() / delete() sonrası çağrılır.
// Sayaç 10'a ulaştıysa veya buffer doluysa GC tetikler.
void maybeAutoGC(void);

#endif