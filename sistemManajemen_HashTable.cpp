#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
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

unordered_map<string, Jadwal> dataJadwal;

size_t getMemoryUsage() {
    size_t size_map_struct = sizeof(dataJadwal);
    size_t size_buckets = dataJadwal.bucket_count() * sizeof(void*);
    size_t node_overhead = sizeof(std::pair<const string, Jadwal>) + sizeof(void*);
    size_t size_nodes = dataJadwal.size() * node_overhead;
    
    size_t size_dynamic_strings = 0;
    for (const auto &pair : dataJadwal) {
        size_dynamic_strings += pair.first.capacity();
        const Jadwal &j = pair.second;
        size_dynamic_strings += j.room_id.capacity();
        size_dynamic_strings += j.room_name.capacity();
        size_dynamic_strings += j.schedule_id.capacity();
        size_dynamic_strings += j.date.capacity();
        size_dynamic_strings += j.activity.capacity();
        size_dynamic_strings += j.status.capacity();
    }
    return size_map_struct + size_buckets + size_nodes + size_dynamic_strings;
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

bool cekKonflik(string room_id,
                string date,
                int startTime,
                int endTime)
{
    for (const auto &pair : dataJadwal) {

        const Jadwal &j = pair.second;

        if (j.room_id == room_id &&
            j.date == date &&
            startTime < j.end_time &&
            endTime > j.start_time)
        {
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

        dataJadwal[j.schedule_id] = j;
    }

    file.close();

    auto end = Clock::now();
    auto dur = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "Jumlah data dimuat: " << dataJadwal.size() << endl;
    cout << endl <<"Runtime loadCSV: " << dur << " milliseconds\n";

    size_t total_memory_bytes = getMemoryUsage();
    cout << "===== SPACE COMPLEXITY (Hash Table) =====\n";
    cout << "Total Penggunaan Memori : " << total_memory_bytes << " bytes ("
         << fixed << setprecision(2) << (double)total_memory_bytes / 1024.0 << " KB / "
         << (double)total_memory_bytes / (1024.0 * 1024.0) << " MB)\n";
    cout << "=========================================\n";
}

void tampilkanJadwal() {

    if (dataJadwal.empty()) {
        cout << "Belum ada data jadwal\n";
        return;
    }

    auto start = Clock::now();

    for (const auto &pair : dataJadwal) {

        // hanya traversal
        volatile auto temp = pair.second.schedule_id;
    }

    auto end = Clock::now();

    cout << "\n===== DATA JADWAL =====\n";

    for (const auto &pair : dataJadwal) {

        const Jadwal &j = pair.second;

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

    cout << "\nRuntime Traversal Hash Table : "
         << chrono::duration_cast<
                chrono::microseconds
            >(end - start).count()
         << " microseconds\n";
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

    // Benchmark cek konflik
    auto startKonflik = Clock::now();

    bool konflik = cekKonflik(
        j.room_id,
        j.date,
        j.start_time,
        j.end_time
    );

    auto endKonflik = Clock::now();

    if (konflik) {

        cout << "Konflik jadwal terdeteksi!\n";
        return;
    }

    cout << "Nama kegiatan: ";
    cin.ignore();
    getline(cin, j.activity);

    j.status = "Booked";

    size_t memBefore = getMemoryUsage();

    // Benchmark insert hash table
    auto startInsert = Clock::now();

    dataJadwal[j.schedule_id] = j;

    auto endInsert = Clock::now();

    size_t memAfter = getMemoryUsage();

    cout << "Jadwal berhasil ditambahkan\n";

    cout << "\nRuntime cekKonflik : "
         << chrono::duration_cast<
                chrono::microseconds
            >(endKonflik - startKonflik).count()
         << " microseconds\n";

    cout << "Runtime insert Hash Table : "
         << chrono::duration_cast<
                chrono::microseconds
            >(endInsert - startInsert).count()
         << " microseconds\n";
    cout << "Space Complexity (Delta): +" << (int)(memAfter - memBefore) << " bytes (Before: " << memBefore << ", After: " << memAfter << " bytes)\n";
}

void searchJadwal()
{
    int pilihan;
    size_t total_hasil_mem = 0;

    cout << "\nCari berdasarkan:\n";
    cout << "1. ID Ruang\n";
    cout << "2. Tanggal (YYYY-MM-DD)\n";
    cout << "3. ID Jadwal\n";
    cout << "Pilih: ";
    cin >> pilihan;

    bool ditemukan = false;

    // ==========================
    // SEARCH BERDASARKAN RUANG
    // ==========================
    if (pilihan == 1)
    {
        string room;

        cout << "Masukkan ID Ruang: ";
        cin >> room;

        vector<Jadwal> hasil;

        auto start = Clock::now();

        for (const auto &pair : dataJadwal)
        {
            if (pair.second.room_id == room)
            {
                hasil.push_back(pair.second);
            }
        }

        auto end = Clock::now();

        for (const auto &j : hasil)
        {
            ditemukan = true;

            cout << "\n----------------------------\n";
            cout << "ID Jadwal : " << j.schedule_id << endl;
            cout << "Ruang     : " << j.room_name << endl;
            cout << "Tanggal   : " << j.date << endl;
            cout << "Mulai     : " << j.start_time << endl;
            cout << "Selesai   : " << j.end_time << endl;
            cout << "Kegiatan  : " << j.activity << endl;
            cout << "Status    : " << j.status << endl;
        }

        if (!ditemukan)
            cout << "\nData tidak ditemukan\n";

        cout << "\nRuntime Search Ruang : "
             << chrono::duration_cast<chrono::microseconds>(end - start).count()
             << " microseconds\n";

        size_t size_hasil_struct = sizeof(hasil);
        size_t size_hasil_elements = hasil.capacity() * sizeof(Jadwal);
        size_t size_hasil_strings = 0;
        for (const auto &j : hasil) {
            size_hasil_strings += j.room_id.capacity() + j.room_name.capacity() + j.schedule_id.capacity() + j.date.capacity() + j.activity.capacity() + j.status.capacity();
        }
        total_hasil_mem = size_hasil_struct + size_hasil_elements + size_hasil_strings;
    }

    // ==========================
    // SEARCH BERDASARKAN TANGGAL
    // ==========================
    else if (pilihan == 2)
    {
        string tanggal;

        cout << "Masukkan Tanggal (YYYY-MM-DD): ";
        cin >> tanggal;

        vector<Jadwal> hasil;

        auto start = Clock::now();

        for (const auto &pair : dataJadwal)
        {
            if (pair.second.date == tanggal)
            {
                hasil.push_back(pair.second);
            }
        }

        auto end = Clock::now();

        for (const auto &j : hasil)
        {
            ditemukan = true;

            cout << "\n----------------------------\n";
            cout << "ID Jadwal : " << j.schedule_id << endl;
            cout << "Ruang     : " << j.room_name << endl;
            cout << "Tanggal   : " << j.date << endl;
            cout << "Mulai     : " << j.start_time << endl;
            cout << "Selesai   : " << j.end_time << endl;
            cout << "Kegiatan  : " << j.activity << endl;
            cout << "Status    : " << j.status << endl;
        }

        if (!ditemukan)
            cout << "\nData tidak ditemukan\n";

        cout << "\nRuntime Search Tanggal : "
             << chrono::duration_cast<chrono::microseconds>(end - start).count()
             << " microseconds\n";

        size_t size_hasil_struct = sizeof(hasil);
        size_t size_hasil_elements = hasil.capacity() * sizeof(Jadwal);
        size_t size_hasil_strings = 0;
        for (const auto &j : hasil) {
            size_hasil_strings += j.room_id.capacity() + j.room_name.capacity() + j.schedule_id.capacity() + j.date.capacity() + j.activity.capacity() + j.status.capacity();
        }
        total_hasil_mem = size_hasil_struct + size_hasil_elements + size_hasil_strings;
    }

    // ==========================
    // SEARCH BERDASARKAN ID JADWAL
    // ==========================
    else if (pilihan == 3)
    {
        string id;

        cout << "Masukkan ID Jadwal: ";
        cin >> id;

        auto start = Clock::now();

        auto it = dataJadwal.find(id);

        auto end = Clock::now();

        if (it != dataJadwal.end())
        {
            ditemukan = true;

            const Jadwal &j = it->second;

            cout << "\n----------------------------\n";
            cout << "ID Jadwal : " << j.schedule_id << endl;
            cout << "Ruang     : " << j.room_name << endl;
            cout << "Tanggal   : " << j.date << endl;
            cout << "Mulai     : " << j.start_time << endl;
            cout << "Selesai   : " << j.end_time << endl;
            cout << "Kegiatan  : " << j.activity << endl;
            cout << "Status    : " << j.status << endl;
        }
        else
        {
            cout << "\nData tidak ditemukan\n";
        }

        cout << "\nRuntime Search ID Jadwal : "
             << chrono::duration_cast<chrono::microseconds>(end - start).count()
             << " microseconds\n";

        total_hasil_mem = sizeof(it);
    }

    else
    {
        cout << "Pilihan tidak valid\n";
    }

    if (pilihan >= 1 && pilihan <= 3) {
        cout << "Space Complexity (Temporary Search Result): " << total_hasil_mem << " bytes\n";
    }
}

void updateJadwal() {

    string id;

    cout << "Masukkan ID Jadwal yang ingin diupdate: ";
    cin >> id;

    auto it = dataJadwal.find(id);

    if (it == dataJadwal.end()) {
        cout << "Jadwal tidak ditemukan\n";
        return;
    }

    int startBaru, endBaru;
    string kegiatanBaru;

    cout << "Waktu mulai baru : ";
    cin >> startBaru;

    cout << "Waktu selesai baru : ";
    cin >> endBaru;

    cout << "Nama kegiatan baru : ";
    cin.ignore();
    getline(cin, kegiatanBaru);

    size_t memBefore = getMemoryUsage();
    auto start = Clock::now();

    it->second.start_time = startBaru;
    it->second.end_time = endBaru;
    it->second.activity = kegiatanBaru;

    auto end = Clock::now();
    size_t memAfter = getMemoryUsage();

    cout << "\nRuntime updateJadwal : "<< chrono::duration_cast<chrono::microseconds>(end-start).count()<< " microseconds\n";
    cout << "Space Complexity (Delta): " << (int)memAfter - (int)memBefore << " bytes (Before: " << memBefore << ", After: " << memAfter << " bytes)\n";

    cout << "Jadwal berhasil diupdate\n";
}

void deleteJadwal() {

    string id;

    cout << "Masukkan ID Jadwal yang ingin dihapus: ";
    cin >> id;

    size_t memBefore = getMemoryUsage();
    auto start = Clock::now();

    auto it = dataJadwal.find(id);
    bool found = false;

    if (it != dataJadwal.end()) {
        dataJadwal.erase(it);
        found = true;
    }

    auto end = Clock::now();
    size_t memAfter = getMemoryUsage();

    if (found) {
        cout << "Jadwal berhasil dihapus\n";
    } else {
        cout << "Jadwal tidak ditemukan\n";
    }

    cout << "\nRuntime deleteJadwal : "
         << chrono::duration_cast<chrono::microseconds>(end-start).count()
         << " microseconds\n";
    cout << "Space Complexity (Delta): " << (int)memAfter - (int)memBefore << " bytes (Before: " << memBefore << ", After: " << memAfter << " bytes)\n";
}

int main() {

    loadCSV("jadwal_ruang_50000data.csv");

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