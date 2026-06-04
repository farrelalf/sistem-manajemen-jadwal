#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace std;
using Clock = chrono::high_resolution_clock;

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

vector<Jadwal> dataJadwal;

size_t getMemoryUsage() {
    size_t size_vector_struct = sizeof(dataJadwal);
    size_t size_elements_reserved = dataJadwal.capacity() * sizeof(Jadwal);
    size_t size_dynamic_strings = 0;
    for (const auto &j : dataJadwal) {
        size_dynamic_strings += j.room_id.capacity();
        size_dynamic_strings += j.room_name.capacity();
        size_dynamic_strings += j.schedule_id.capacity();
        size_dynamic_strings += j.date.capacity();
        size_dynamic_strings += j.activity.capacity();
        size_dynamic_strings += j.status.capacity();
    }
    return size_vector_struct + size_elements_reserved + size_dynamic_strings;
}

const vector<string> validRooms = {"R001","R002","R003","R004","R005","R006"};

string getRoomName(const string &id) {
    if (id == "R001") return "RK A1";
    if (id == "R002") return "RK A2";
    if (id == "R003") return "RK B1";
    if (id == "R004") return "RK B4";
    if (id == "R005") return "Lab Komputer 1";
    if (id == "R006") return "Lab Komputer 2";
    return "Unknown";
}

bool isValidRoom(const string &id) {
    for (auto &r : validRooms) {
        if (r == id) return true;
    }
    return false;
}

bool cekKonflik(string room_id, string date, int start, int end) {

    for (const auto &j : dataJadwal) {

        if (j.room_id == room_id && j.date == date) {

            if (start < j.end_time && end > j.start_time)
                return true;
        }
    }

    return false;
}

void loadCSV(string filename) {

    auto start = Clock::now();

    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cout << "File CSV tidak ditemukan\n";
        return;
    }

    getline(file, line);

    while (getline(file, line)) {

        stringstream ss(line);
        string item;

        Jadwal j;

        getline(ss, j.room_id, ',');
        getline(ss, j.room_name, ',');
        j.room_name = getRoomName(j.room_id);
        getline(ss, j.schedule_id, ',');
        getline(ss, j.date, ',');

        getline(ss, item, ',');
        j.start_time = stoi(item);

        getline(ss, item, ',');
        j.end_time = stoi(item);

        getline(ss, j.activity, ',');
        getline(ss, j.status, ',');

        dataJadwal.push_back(j);
    }

    file.close();

    auto end = Clock::now();
    auto dur = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "Jumlah data dimuat: " << dataJadwal.size() << endl;
    cout << endl <<"Runtime loadCSV: " << dur << " milliseconds\n";

    size_t total_memory_bytes = getMemoryUsage();
    cout << "===== SPACE COMPLEXITY (Vektor) =====\n";
    cout << "Total Penggunaan Memori : " << total_memory_bytes << " bytes ("
         << fixed << setprecision(2) << (double)total_memory_bytes / 1024.0 << " KB / "
         << (double)total_memory_bytes / (1024.0 * 1024.0) << " MB)\n";
    cout << "======================================\n";
}

void tampilkanJadwal() {

    if (dataJadwal.empty()) {
        cout << "Belum ada data jadwal\n";
        return;
    }

    auto start = Clock::now();

    for (const auto &j : dataJadwal) {
        // hanya traversal
        volatile auto temp = j.schedule_id;
    }

    auto end = Clock::now();

    for (const auto &j : dataJadwal) {

        cout << "\n----------------------------\n";
        cout << "ID Ruangan : " << j.room_id << endl;
        cout << "Ruang      : " << j.room_name << endl;
        cout << "ID Jadwal  : " << j.schedule_id << endl;
        cout << "Tanggal    : " << j.date << endl;
        cout << "Mulai      : " << j.start_time << endl;
        cout << "Selesai    : " << j.end_time << endl;
        cout << "Kegiatan   : " << j.activity << endl;
        cout << "Status     : " << j.status << endl;
    }

    auto dur = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << "\nRuntime Traversal Vector : " << dur << " microseconds\n";
    cout << "Space Complexity (Traversal): " << getMemoryUsage() << " bytes\n";
}

void insertJadwal() {

    Jadwal j;

    cout << "ID Ruang (R001-R006): ";
    cin >> j.room_id;

    if (!isValidRoom(j.room_id)) {
        cout << "Error: ID Ruang tidak valid.\n";
        return;
    }
    
    j.room_name = getRoomName(j.room_id);

    cout << "ID Jadwal: ";
    cin >> j.schedule_id;

    cout << "Tanggal (YYYY-MM-DD): ";
    cin >> j.date;

    cout << "Waktu mulai (contoh 800): ";
    cin >> j.start_time;

    cout << "Waktu selesai: ";
    cin >> j.end_time;

    auto startKonflik = Clock::now();
    bool konflik = cekKonflik(j.room_id, j.date, j.start_time, j.end_time);
    auto endKonflik = Clock::now();

    if (konflik) {
        cout << "Konflik jadwal terdeteksi!\n";
        auto durKonflik = chrono::duration_cast<chrono::microseconds>(endKonflik - startKonflik).count();
        cout << "\nRuntime cekKonflik : " << durKonflik << " microseconds\n";
        return;
    }

    cout << "Nama kegiatan: ";
    
    cin.ignore();
    getline(cin, j.activity);

    j.status = "Booked";

    size_t memBefore = getMemoryUsage();

    auto startInsert = Clock::now();
    dataJadwal.push_back(j);
    auto endInsert = Clock::now();

    size_t memAfter = getMemoryUsage();

    cout << "Jadwal berhasil ditambahkan\n";

    auto durKonflik = chrono::duration_cast<chrono::microseconds>(endKonflik - startKonflik).count();
    auto durInsert = chrono::duration_cast<chrono::microseconds>(endInsert - startInsert).count();

    cout << "\nRuntime cekKonflik : " << durKonflik << " microseconds\n";
    cout << "Runtime insert Vector : " << durInsert << " microseconds\n";
    cout << "Space Complexity (Delta): +" << (int)(memAfter - memBefore) << " bytes (Before: " << memBefore << ", After: " << memAfter << " bytes)\n";
}

void searchJadwal() {

    int mode;

    cout << "Cari berdasarkan:\n";
    cout << "1. ID Ruang\n";
    cout << "2. Tanggal (YYYY-MM-DD)\n";
    cout << "3. ID Jadwal\n";
    cout << "Pilih: ";
    cin >> mode;

    string key;
    vector<Jadwal> hasil;

    if (mode == 1) {
        cout << "Masukkan ID Ruang: ";
        cin >> key;

        auto start = Clock::now();

        for (const auto &j : dataJadwal) {
            if (j.room_id == key) {
                hasil.push_back(j);
            }
        }

        auto end = Clock::now();

        // for (const auto &j : hasil) {
        //     cout << "\n----------------------------\n";
        //     cout << "ID Jadwal : " << j.schedule_id << endl;
        //     cout << "Ruang     : " << j.room_name << endl;
        //     cout << "Tanggal   : " << j.date << endl;
        //     cout << "Mulai     : " << j.start_time << endl;
        //     cout << "Selesai   : " << j.end_time << endl;
        //     cout << "Kegiatan  : " << j.activity << endl;
        // }

        if (hasil.empty())
            cout << "Data tidak ditemukan\n";

        auto dur = chrono::duration_cast<chrono::microseconds>(end - start).count();
        cout << "\nRuntime Search Ruang : " << dur << " microseconds\n";
    }
    else if (mode == 2) {
        cout << "Masukkan Tanggal: ";
        cin >> key;

        auto start = Clock::now();

        for (const auto &j : dataJadwal) {
            if (j.date == key) {
                hasil.push_back(j);
            }
        }

        auto end = Clock::now();

        for (const auto &j : hasil) {
            cout << "\n----------------------------\n";
            cout << "ID Jadwal : " << j.schedule_id << endl;
            cout << "Ruang     : " << j.room_name << endl;
            cout << "Tanggal   : " << j.date << endl;
            cout << "Mulai     : " << j.start_time << endl;
            cout << "Selesai   : " << j.end_time << endl;
            cout << "Kegiatan  : " << j.activity << endl;
        }

        if (hasil.empty())
            cout << "Data tidak ditemukan\n";

        auto dur = chrono::duration_cast<chrono::microseconds>(end - start).count();
        cout << "\nRuntime Search Tanggal : " << dur << " microseconds\n";
    }
    else if (mode == 3) {
        cout << "Masukkan ID Jadwal: ";
        cin >> key;

        auto start = Clock::now();

        for (const auto &j : dataJadwal) {
            if (j.schedule_id == key) {
                hasil.push_back(j);
            }
        }

        auto end = Clock::now();

        for (const auto &j : hasil) {
            cout << "\n----------------------------\n";
            cout << "ID Jadwal : " << j.schedule_id << endl;
            cout << "Ruang     : " << j.room_name << endl;
            cout << "Tanggal   : " << j.date << endl;
            cout << "Mulai     : " << j.start_time << endl;
            cout << "Selesai   : " << j.end_time << endl;
            cout << "Kegiatan  : " << j.activity << endl;
        }

        if (hasil.empty())
            cout << "Data tidak ditemukan\n";

        auto dur = chrono::duration_cast<chrono::microseconds>(end - start).count();
        cout << "\nRuntime Search ID Jadwal : " << dur << " microseconds\n";
    }

    // Hitung Space Complexity Hasil Pencarian Sementara
    size_t size_hasil_struct = sizeof(hasil);
    size_t size_hasil_elements = hasil.capacity() * sizeof(Jadwal);
    size_t size_hasil_strings = 0;
    for (const auto &j : hasil) {
        size_hasil_strings += j.room_id.capacity() + j.room_name.capacity() + j.schedule_id.capacity() + j.date.capacity() + j.activity.capacity() + j.status.capacity();
    }
    size_t total_hasil_mem = size_hasil_struct + size_hasil_elements + size_hasil_strings;
    cout << "Space Complexity (Temporary Search Result): " << total_hasil_mem << " bytes\n";
}

void updateJadwal() {

    string id;

    cout << "Masukkan ID Jadwal yang ingin diupdate: ";
    cin >> id;

    int startBaru, endBaru;
    string kegiatanBaru;

    cout << "Waktu mulai baru: ";
    cin >> startBaru;

    cout << "Waktu selesai baru: ";
    cin >> endBaru;

    cout << "Nama kegiatan baru: ";
    cin.ignore();
    getline(cin, kegiatanBaru);

    size_t memBefore = getMemoryUsage();
    auto start = Clock::now();
    bool found = false;

    for (auto &j : dataJadwal) {
        if (j.schedule_id == id) {
            j.start_time = startBaru;
            j.end_time = endBaru;
            j.activity = kegiatanBaru;
            found = true;
            break;
        }
    }

    auto end = Clock::now();
    size_t memAfter = getMemoryUsage();

    if (found) {
        cout << "Jadwal berhasil diupdate\n";
    } else {
        cout << "Jadwal tidak ditemukan\n";
    }

    auto dur = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "\nRuntime updateJadwal: " << dur << " microseconds\n";
    cout << "Space Complexity (Delta): " << (int)memAfter - (int)memBefore << " bytes (Before: " << memBefore << ", After: " << memAfter << " bytes)\n";
}

void deleteJadwal() {

    string id;

    cout << "Masukkan ID Jadwal yang ingin dihapus: ";
    cin >> id;

    size_t memBefore = getMemoryUsage();
    auto start = Clock::now();
    bool found = false;

    for (int i = 0; i < dataJadwal.size(); i++) {
        if (dataJadwal[i].schedule_id == id) {
            dataJadwal.erase(dataJadwal.begin() + i);
            found = true;
            break;
        }
    }

    auto end = Clock::now();
    size_t memAfter = getMemoryUsage();

    if (found) {
        cout << "Jadwal berhasil dihapus\n";
    } else {
        cout << "Jadwal tidak ditemukan\n";
    }

    auto dur = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "\nRuntime deleteJadwal: " << dur << " microseconds\n";
    cout << "Space Complexity (Delta): " << (int)memAfter - (int)memBefore << " bytes (Before: " << memBefore << ", After: " << memAfter << " bytes)\n";
}

int main() {

    loadCSV("jadwal_ruang_3600data.csv");

    int pilihan;

    do {

        cout << "\n===== SISTEM MANAJEMEN JADWAL RUANG =====\n";
        cout << "1. Tampilkan Jadwal\n";
        cout << "2. Insert Jadwal\n";
        cout << "3. Search Jadwal Berdasarkan Ruang/Tanggal\n";
        cout << "4. Update Jadwal\n";
        cout << "5. Delete Jadwal\n";
        cout << "6. Keluar\n";

        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {

        case 1:
            tampilkanJadwal();
            break;

        case 2:
            insertJadwal();
            break;

        case 3:
            searchJadwal();
            break;

        case 4:
            updateJadwal();
            break;

        case 5:
            deleteJadwal();
            break;
        }

    } while (pilihan != 6);
}