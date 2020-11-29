#ifndef __INVENTORY_ARRAY__
#define __INVENTORY_ARRAY__
#include "../boolean/boolean.h"

#define InitialSize 50

typedef int IdxType;
typedef struct {
    char *Nama;
    int Jumlah;
} ElType;

typedef struct {
    ElType *A;
    int Capacity;
    int Neff;
} ArrayInventory;

/**
 * Konstruktor
 * I.S. sembarang
 * F.S. Terbentuk List kosong dengan ukuran InitialSize
 */
ArrayInventory MakeArrayInventory();

/**
 * Destruktor
 * I.S. List terdefinisi
 * F.S. list->A terdealokasi
 */
void DeallocateArrayInventory(ArrayInventory *list);

/**
 * Fungsi untuk mengetahui apakah suatu list kosong.
 * Prekondisi: list terdefinisi
 */
boolean IsArrayInventoryEmpty(ArrayInventory list);

/**
 * Fungsi untuk mendapatkan banyaknya elemen efektif list, 0 jika tabel kosong.
 * Prekondisi: list terdefinisi
 */
int ArrayInventoryLength(ArrayInventory list);

/**
 * Mengembalikan elemen list L yang ke-I (indeks lojik).
 * Prekondisi: list tidak kosong, i di antara 0..Length(list).
 */
ElType ArrayInventoryGet(ArrayInventory list, IdxType i);

/**
 * Fungsi untuk mendapatkan kapasitas yang tersedia.
 * Prekondisi: list terdefinisi
 */
int ArrayInventoryGetCapacity(ArrayInventory list);

/**
 * Fungsi untuk menambahkan elemen baru di index ke-i
 * Prekondisi: list terdefinisi, i di antara 0..Length(list).
 */
void ArrayInventoryInsertAt(ArrayInventory *list, ElType el, IdxType i);

/**
 * Fungsi untuk menambahkan elemen baru di akhir list.
 * Prekondisi: list terdefinisi
 */
void ArrayInventoryInsertLast(ArrayInventory *list, ElType el);

/**
 * Fungsi untuk menambahkan elemen baru di awal list.
 * Prekondisi: list terdefinisi
 */
void ArrayInventoryInsertFirst(ArrayInventory *list, ElType el);

#endif	
