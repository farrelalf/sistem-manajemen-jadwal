#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>

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

    // Benchmark insert hash table
    auto startInsert = Clock::now();

    dataJadwal[j.schedule_id] = j;

    auto endInsert = Clock::now();

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
}

void searchJadwal()
{
    int pilihan;

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
    }

    else
    {
        cout << "Pilihan tidak valid\n";
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

    auto start = Clock::now();

    it->second.start_time = startBaru;
    it->second.end_time = endBaru;
    it->second.activity = kegiatanBaru;

    auto end = Clock::now();
    cout << "\nRuntime updateJadwal : "<< chrono::duration_cast<chrono::microseconds>(end-start).count()<< " microseconds\n";

     cout << "Jadwal berhasil diupdate\n";
}

void deleteJadwal() {

    string id;

    cout << "Masukkan ID Jadwal yang ingin dihapus: ";
    cin >> id;

    auto start = Clock::now();

    auto it = dataJadwal.find(id);

    if (it != dataJadwal.end()) {

        dataJadwal.erase(it);

        cout << "Jadwal berhasil dihapus\n";

    } else {

        cout << "Jadwal tidak ditemukan\n";
    }

    auto end = Clock::now();

    cout << "\nRuntime deleteJadwal : "
         << chrono::duration_cast<chrono::microseconds>(end-start).count()
         << " microseconds\n";
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