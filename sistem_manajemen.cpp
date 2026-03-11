#include <iostream>
#include <vector>
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

vector<Jadwal> dataJadwal;

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
}

void tampilkanJadwal() {

    auto start = Clock::now();

    if (dataJadwal.empty()) {
        cout << "Belum ada data jadwal\n";
        return;
    }

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

    auto end = Clock::now();
    auto dur = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "\nRuntime tampilkanJadwal: " << dur << " milliseconds\n";
}

void insertJadwal() {

    Jadwal j;

    cout << "ID Ruang (R001-R006): ";
    cin >> j.room_id;

    if (!isValidRoom(j.room_id)) {
        cout << "Error: ID Ruang tidak valid.\n";
        return;
    }

    // fill room name automatically based on ID
    j.room_name = getRoomName(j.room_id);

    cout << "ID Jadwal: ";
    cin >> j.schedule_id;

    cout << "Tanggal (YYYY-MM-DD): ";
    cin >> j.date;

    cout << "Waktu mulai (contoh 800): ";
    cin >> j.start_time;

    cout << "Waktu selesai: ";
    cin >> j.end_time;

    auto start = Clock::now();

    if (cekKonflik(j.room_id, j.date, j.start_time, j.end_time)) {

        cout << "Konflik jadwal terdeteksi!\n";

    } else {
        cout << "Nama kegiatan: ";
        cin.ignore();
        getline(cin, j.activity);

        j.status = "Booked";
        dataJadwal.push_back(j);

        cout << "Jadwal berhasil ditambahkan\n";
    }

    auto end = Clock::now();
    auto dur = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "\nRuntime insertJadwal: " << dur << " milliseconds\n";
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

    if (mode == 1)
        cout << "Masukkan ID Ruang: ";
    else if (mode == 2)
        cout << "Masukkan Tanggal: ";
    else if (mode == 3)
        cout << "Masukkan ID Jadwal: ";
    else
        return;

    cin >> key;

    auto start = Clock::now();

    bool found = false;

    for (const auto &j : dataJadwal) {

        if ((mode == 1 && j.room_id == key) ||
            (mode == 2 && j.date == key) ||
            (mode == 3 && j.schedule_id == key)) {

            cout << "\n----------------------------\n";
            cout << "ID Jadwal : " << j.schedule_id << endl;
            cout << "Ruang     : " << j.room_name << endl;
            cout << "Tanggal   : " << j.date << endl;
            cout << "Mulai     : " << j.start_time << endl;
            cout << "Selesai   : " << j.end_time << endl;
            cout << "Kegiatan  : " << j.activity << endl;

            found = true;
        }
    }

    if (!found)
        cout << "Data tidak ditemukan\n";

    auto end = Clock::now();
    auto dur = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "\nRuntime searchJadwal: " << dur << " milliseconds\n";
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

    auto start = Clock::now();

    for (auto &j : dataJadwal) {

        if (j.schedule_id == id) {

            j.start_time = startBaru;
            j.end_time = endBaru;
            j.activity = kegiatanBaru;

            cout << "Jadwal berhasil diupdate\n";

            auto end = Clock::now();
            auto dur = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            cout << "\nRuntime updateJadwal: " << dur << " milliseconds\n";
            return;
        }
    }

    cout << "Jadwal tidak ditemukan\n";

    auto end = Clock::now();
    auto dur = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "\nRuntime updateJadwal: " << dur << " milliseconds\n";
}

void deleteJadwal() {

    string id;

    cout << "Masukkan ID Jadwal yang ingin dihapus: ";
    cin >> id;

    auto start = Clock::now();

    for (int i = 0; i < dataJadwal.size(); i++) {

        if (dataJadwal[i].schedule_id == id) {

            dataJadwal.erase(dataJadwal.begin() + i);

            cout << "Jadwal berhasil dihapus\n";

            auto end = Clock::now();
            auto dur = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            cout << "\nRuntime deleteJadwal: " << dur << " milliseconds\n";
            return;
        }
    }

    cout << "Jadwal tidak ditemukan\n";

    auto end = Clock::now();
    auto dur = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "\nRuntime deleteJadwal: " << dur << " milliseconds\n";
}

int main() {

    loadCSV("jadwal_ruang_1semester_3600data.csv");

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