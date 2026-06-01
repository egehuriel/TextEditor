#ifndef OPERATIONS_H
#define OPERATIONS_H

/*
 * insert(index)
 * ─────────────
 * Kullanıcıdan yeni satır içeriğini alır ve onu `index` numaralı satırın
 * ÖNÜNE (yani ondan önce) ekler.  index == NIL ise listeye ilk eleman
 * olarak eklenir.
 *
 * Fiziksel yer   : textbuffer[freeIndex]
 * Bağlantılar    : yeni node'un prev/next'i ve komşu node'ların next/prev'i
 *                  güncellenir.
 * Dönüş değeri   : eklenen node'un textbuffer indeksi, hata durumunda -1.
 */
int insert(int index);

/*
 * delete(index)
 * ─────────────
 * `index` numaralı node'u bağlı listeden koparır (next/prev bağlarını
 * günceller).  Node'un içeriği silinmez; GC tarafından temizlenecektir.
 *
 * Dönüş değeri   : 0 başarı, -1 hata (geçersiz index).
 */
int deleteNode(int index);

/*
 * replace(index, col)
 * ───────────────────
 * `index` numaralı node'un `col` sütunundaki TEK karakteri değiştirir.
 * Eski karakter, yeni karakter ve satırın tamamı ekrana yazdırılır.
 *
 * Dönüş değeri   : 0 başarı, -1 hata.
 */
int replace(int index, int col);

#endif /* OPERATIONS_H */
