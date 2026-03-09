#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

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

    cout << "Dataset berhasil dimuat!\n";
}

void tampilkanJadwal() {

    if (dataJadwal.empty()) {
        cout << "Belum ada data jadwal\n";
        return;
    }

    for (auto j : dataJadwal) {

        cout << "\n----------------------------\n";
        cout << "ID Jadwal : " << j.schedule_id << endl;
        cout << "Ruang     : " << j.room_name << endl;
        cout << "Tanggal   : " << j.date << endl;
        cout << "Mulai     : " << j.start_time << endl;
        cout << "Selesai   : " << j.end_time << endl;
        cout << "Kegiatan  : " << j.activity << endl;
        cout << "Status    : " << j.status << endl;
    }
}

void insertJadwal() {

    Jadwal j;

    cout << "ID Ruang: ";
    cin >> j.room_id;

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

    cout << "Nama kegiatan: ";
    cin.ignore();
    getline(cin, j.activity);

    j.status = "Booked";

    if (cekKonflik(j.room_id, j.date, j.start_time, j.end_time)) {
        cout << "Konflik jadwal terdeteksi!\n";
    }
    else {

        dataJadwal.push_back(j);
        cout << "Jadwal berhasil ditambahkan\n";
    }
}

void searchByRoom() {

    string id;

    cout << "Masukkan ID Ruang: ";
    cin >> id;

    for (auto j : dataJadwal) {

        if (j.room_id == id) {

            cout << "\nID Jadwal : " << j.schedule_id;
            cout << "\nTanggal   : " << j.date;
            cout << "\nMulai     : " << j.start_time;
            cout << "\nSelesai   : " << j.end_time;
            cout << "\nKegiatan  : " << j.activity << endl;
        }
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

    loadCSV("jadwal_ruang.csv");

    int pilihan;

    do {

        cout << "\n===== SISTEM MANAJEMEN JADWAL RUANG =====\n";
        cout << "1. Tampilkan Jadwal\n";
        cout << "2. Insert Jadwal\n";
        cout << "3. Search Jadwal Berdasarkan Ruang\n";
        cout << "4. Update Jadwal\n";
        cout << "5. Delete Jadwal\n";
        cout << "6. Exit\n";

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
            searchByRoom();
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