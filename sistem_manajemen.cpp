#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>  

using namespace std;
using Clock = std::chrono::high_resolution_clock;

template<typename Func>
void measureRuntime(const string &name, Func f) {
    auto start = Clock::now();
    f();
    auto end = Clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    cout << endl <<"Runtime " << name << ": "<< dur << "ms" << endl;
}

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

// only these room IDs are permitted when inserting
const vector<string> validRooms = {"R001","R002","R003","R004","R005","R006"};

// helper to check if a room id is allowed
bool isValidRoom(const string &id) {
    for (auto &r : validRooms) {
        if (r == id) return true;
    }
    return false;
}

bool cekKonflik(string room_id, string date, int start, int end) {

    for (auto j : dataJadwal) {

        if (j.room_id == room_id && j.date == date) {

            if (start < j.end_time && end > j.start_time) {
                return true;
            }
        }
    }

    return false;
}

void loadCSV(string filename) {

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
}

void tampilkanJadwal() {

    if (dataJadwal.empty()) {
        cout << "Belum ada data jadwal\n";
        return;
    }

    for (auto j : dataJadwal) {

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
}

void insertJadwal() {

    Jadwal j;

    cout << "ID Ruang (R001-R006): ";
    cin >> j.room_id;
    if (!isValidRoom(j.room_id)) {
        cout << "Error: ID Ruang tidak valid. Hanya R001..R006 yang diperbolehkan.\n";
        return;
    }

    cout << "Nama Ruang: ";
    cin.ignore();
    getline(cin, j.room_name);

    cout << "ID Jadwal: ";
    cin >> j.schedule_id;

    cout << "Tanggal (YYYY-MM-DD): ";
    cin >> j.date;

    cout << "Waktu mulai (contoh 800): ";
    cin >> j.start_time;

    cout << "Waktu selesai: ";
    cin >> j.end_time;

    if (cekKonflik(j.room_id, j.date, j.start_time, j.end_time)) {
        cout << "Konflik jadwal terdeteksi!\n";
    }
    else {
        cout << "Nama kegiatan: ";
        cin.ignore();
        getline(cin, j.activity);

        j.status = "Booked";

        dataJadwal.push_back(j);
        cout << "Jadwal berhasil ditambahkan\n";
    }
}

void searchJadwal() {
    int mode;
    cout << "Cari berdasarkan:\n";
    cout << "1. ID Ruang\n";
    cout << "2. Tanggal (YYYY-MM-DD)\n";
    cout << "Pilih: ";
    cin >> mode;

    string key;
    if (mode == 1) {
        cout << "Masukkan ID Ruang: ";
    } else if(mode == 2) {
        cout << "Masukkan Tanggal: ";
    } else {
        return;
    }
    cin >> key;

    bool found = false;
    for (auto &j : dataJadwal) {
        if ((mode == 1 && j.room_id == key) ||
            (mode == 2 && j.date == key)) {
            cout << "\n----------------------------\n";
            cout << "\nID Jadwal : " << j.schedule_id;
            cout << "\nRuang      : " << j.room_name;
            cout << "\nTanggal    : " << j.date;
            cout << "\nMulai      : " << j.start_time;
            cout << "\nSelesai    : " << j.end_time;
            cout << "\nKegiatan   : " << j.activity << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Data tidak ditemukan\n";
    }
}

void updateJadwal() {

    string id;

    cout << "Masukkan ID Jadwal yang ingin diupdate: ";
    cin >> id;

    for (auto &j : dataJadwal) {

        if (j.schedule_id == id) {

            cout << "Waktu mulai baru: ";
            cin >> j.start_time;

            cout << "Waktu selesai baru: ";
            cin >> j.end_time;

            cout << "Nama kegiatan baru: ";
            cin.ignore();
            getline(cin, j.activity);

            cout << "Jadwal berhasil diupdate\n";
            return;
        }
    }

    cout << "Jadwal tidak ditemukan\n";
}

void deleteJadwal() {

    string id;

    cout << "Masukkan ID Jadwal yang ingin dihapus: ";
    cin >> id;

    for (int i = 0; i < dataJadwal.size(); i++) {

        if (dataJadwal[i].schedule_id == id) {

            dataJadwal.erase(dataJadwal.begin() + i);

            cout << "Jadwal berhasil dihapus\n";
            return;
        }
    }

    cout << "Jadwal tidak ditemukan\n";
}

int main() {

    measureRuntime("loadCSV", [](){ loadCSV("jadwal_ruang_1semester_3600data.csv"); });

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
            measureRuntime("tampilkanJadwal", [](){ tampilkanJadwal(); });
            break;

        case 2:
            measureRuntime("insertJadwal", [](){ insertJadwal(); });
            break;

        case 3:
            measureRuntime("searchJadwal", [](){ searchJadwal(); });
            break;

        case 4:
            measureRuntime("updateJadwal", [](){ updateJadwal(); });
            break;

        case 5:
            measureRuntime("deleteJadwal", [](){ deleteJadwal(); });
            break;
        }

    } while (pilihan != 6);

}