# Sistem Manajemen Jadwal & Konflik Ruang

Project ini merupakan implementasi sistem manajemen jadwal penggunaan ruang menggunakan bahasa **C++**. Sistem ini dirancang untuk mengelola jadwal ruang seperti ruang rapat, ruang kelas, atau coworking space serta mendeteksi konflik jadwal berdasarkan waktu penggunaan ruang.

Project ini dibuat sebagai bagian dari tugas **Struktur Data**.

---

## Fitur Sistem

Sistem memiliki beberapa fitur utama:

- Menambahkan jadwal baru (Insert)
- Mencari jadwal berdasarkan ruang (Search)
- Mengupdate jadwal kegiatan (Update)
- Menghapus jadwal (Delete)
- Deteksi konflik jadwal berdasarkan interval waktu
- Membaca dataset jadwal dari file CSV

---

## Struktur Data

Struktur data yang digunakan pada implementasi awal adalah:

- **Vector (Dynamic Array)**

Vector digunakan untuk menyimpan kumpulan data jadwal dan memungkinkan operasi seperti insert, search, update, dan delete.

Contoh struktur data yang digunakan:

```cpp
struct Jadwal {
    string room_id;
    string room_name;
    string schedule_id;
    string date;
    int start_time;
    int end_time;
    string activity;
    string status;
};