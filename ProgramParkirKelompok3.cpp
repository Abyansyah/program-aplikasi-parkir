#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <math.h>
#include <algorithm>

using namespace std;

struct Vehicle {
    string plateNumber;
    string vehicleType;
    time_t entryTime;
    time_t exitTime;
    double tariff;
    string receiptNumber;
};

struct VehicleType {
    string typeName;
    double tariff;
};

vector<Vehicle> transactions;
vector<VehicleType> vehicleTypes = { {"Motor", 2000}, {"Mobil", 5000} };
const int MAX_TRANSACTIONS = 10;

void garis() {
    cout << "======================================================\n";
}

void header(string title) {
    garis();
    cout << "\t\t" << title << "\n";
    garis();
}

string generateReceiptNumber() {
    srand(time(0));
    int randomNumber = rand() % 9000 + 1000;
    return "REC-" + to_string(randomNumber);
}

void listVehicleTypes() {
    header("JENIS KENDARAAN");
    for (size_t i = 0; i < vehicleTypes.size(); ++i) {
        cout << " " << i + 1 << ". " << left << setw(10) << vehicleTypes[i].typeName 
             << "Tarif: Rp" << fixed << setprecision(0) << vehicleTypes[i].tariff << "/jam\n";
    }
}

void listVehicleTypesStop() {
    header("JENIS KENDARAAN");
    for (size_t i = 0; i < vehicleTypes.size(); ++i) {
        cout << " " << i + 1 << ". " << left << setw(10) << vehicleTypes[i].typeName 
             << "Tarif: Rp" << fixed << setprecision(0) << vehicleTypes[i].tariff << "/jam\n";
    }

    cout << "Tekan enter untuk kembali...";
    cin.ignore();
    cin.get();
}

void addVehicleType() {
    VehicleType vt;
    header("TAMBAH JENIS KENDARAAN");
    
    cin.ignore(); 
    cout << " Masukkan nama jenis kendaraan : ";
    getline(cin, vt.typeName);

    transform(vt.typeName.begin(), vt.typeName.end(), vt.typeName.begin(), ::toupper);

    for (const auto& existingType : vehicleTypes) {
        string existingTypeUpper = existingType.typeName;
        transform(existingTypeUpper.begin(), existingTypeUpper.end(), existingTypeUpper.begin(), ::toupper);

        if (existingTypeUpper == vt.typeName) {
            cout << "Jenis kendaraan sudah ditambahkan!\n";
            cout << "Tekan enter untuk kembali..." << endl;
            cin.get(); 
            return;
        }
    }

    cout << " Masukkan tarif per jam  : ";
    while (!(cin >> vt.tariff)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid! Masukkan angka untuk tarif: ";
    }

    vehicleTypes.push_back(vt);
    cout << " Jenis kendaraan berhasil ditambahkan!\n";
}

void removeVehicleType() {
    header("HAPUS JENIS KENDARAAN");
    listVehicleTypes();
    int choice;
    cout << "Pilih jenis yang dihapus (1-" << vehicleTypes.size() << ") : ";
    cin >> choice;
    
    if (choice > 0 && choice <= vehicleTypes.size()) {
        vehicleTypes.erase(vehicleTypes.begin() + choice - 1);
        cout << "Jenis kendaraan berhasil dihapus!\n";
    } else {
        cout << "Pilihan tidak valid!\n";
    }
}

void manageVehicleTypes() {
    int choice;
    do {
        header("MANAJEMEN JENIS KENDARAAN");
        cout << " 1. Tambah Jenis Kendaraan\n";
        cout << " 2. Lihat Jenis Kendaraan\n";
        cout << " 3. Hapus Jenis Kendaraan\n";
        cout << " 4. Kembali\n";
        garis();
        cout << " Pilih menu : ";
        cin >> choice;
        
        switch (choice) {
            case 1: addVehicleType(); break;
            case 2: listVehicleTypesStop(); break;
            case 3: removeVehicleType(); break;
            case 4: cout << "\nKembali ke menu utama...\n"; break;
            default: cout << " Pilihan tidak valid!\n";
        }
    } while (choice != 4);
}

time_t getCurrentTime() {
    return time(0);
}

void printReceipt(const Vehicle& v) {
    header("STRUK PARKIR MASUK");
    cout << " No. Struk      : " << v.receiptNumber << "\n";
    cout << " Plat Nomor     : " << v.plateNumber << "\n";
    cout << " Jenis Kendaraan: " << v.vehicleType << "\n";
    cout << " Waktu Masuk    : " << ctime(&v.entryTime);
    cout << " Tarif         : Rp" << fixed << setprecision(0) << v.tariff << "/jam\n";
    garis();
}

void transactionEntry() {
    if (transactions.size() >= MAX_TRANSACTIONS) {
        header("PEMBERITAHUAN");
        cout << " Kapasitas parkir penuh (" << MAX_TRANSACTIONS << " kendaraan)\n";
        garis();
        return;
    }

    header("TRANSAKSI MASUK");
    string plateNumber;
    cin.ignore();
    cout << " Masukkan plat nomor : ";
    getline(cin, plateNumber);

    for (auto &ch : plateNumber) ch = toupper(ch);

    for (const auto &v : transactions) {
        if (v.plateNumber == plateNumber && v.exitTime == 0) {
            header("PERINGATAN");
            cout << " Kendaraan " << plateNumber << " masih parkir!\n";
            garis();
            cout << " Tekan enter untuk kembali...";
            cin.ignore();
            return;
        }
    }

    Vehicle newVehicle;
    newVehicle.plateNumber = plateNumber;
    
    listVehicleTypes();
    int choice;
    cout << " Pilih jenis kendaraan (1-" << vehicleTypes.size() << ") : ";
    cin >> choice;

    if (choice > 0 && choice <= vehicleTypes.size()) {
        newVehicle.vehicleType = vehicleTypes[choice-1].typeName;
        newVehicle.tariff = vehicleTypes[choice-1].tariff;
        newVehicle.entryTime = getCurrentTime();
        newVehicle.exitTime = 0;
        newVehicle.receiptNumber = generateReceiptNumber();
        
        printReceipt(newVehicle);
        
        char confirm;
        cout << " Konfirmasi simpan transaksi (y/n)? ";
        cin >> confirm;
        
        if (tolower(confirm) == 'y') {
            transactions.push_back(newVehicle);
            cout << " Transaksi berhasil disimpan!\n";
        } else {
            cout << " Transaksi dibatalkan!\n";
        }
    } else {
        cout << " Pilihan tidak valid!\n";
    }
}

void transactionExit() {
    header("TRANSAKSI KELUAR");
    string plateNumber;
    cin.ignore();
    cout << " Masukkan plat nomor : ";
    getline(cin, plateNumber);

    bool found = false;
    for (auto &v : transactions) {
        if (v.plateNumber == plateNumber && v.exitTime == 0) {
            v.exitTime = getCurrentTime();
            double duration = difftime(v.exitTime, v.entryTime)/3600;
            double total = ceil(duration) * v.tariff;

            header("DETAIL TRANSAKSI");
            cout << " Plat Nomor     : " << v.plateNumber << "\n";
            cout << " Jenis Kendaraan: " << v.vehicleType << "\n";
            cout << " Waktu Masuk    : " << ctime(&v.entryTime);
            cout << " Waktu Keluar   : " << ctime(&v.exitTime);
            cout << " Durasi         : " << fixed << setprecision(1) << duration << " jam\n";
            cout << " Total Biaya    : Rp" << fixed << setprecision(0) << total << "\n";
            garis();
            
            char confirm;
            cout << " Konfirmasi transaksi (y/n)? ";
            cin >> confirm;
            
            if (tolower(confirm) != 'y') {
                v.exitTime = 0;
                cout << " Transaksi dibatalkan!\n";
            } else {
                cout << " Transaksi berhasil disimpan!\n";
            }
            found = true;
            break;
        }
    }

    if (!found) {
        header("PEMBERITAHUAN");
        cout << " Data kendaraan tidak ditemukan!\n";
        garis();
    }
}

void listTransactions() {
    header("LAPORAN TRANSAKSI");
    if (transactions.empty()) {
        cout << " Belum ada transaksi\n";
    } else {
        for (const auto &v : transactions) {
            cout << " No. Struk : " << v.receiptNumber << "\n";
            cout << " Plat      : " << v.plateNumber << "\n";
            cout << " Jenis     : " << v.vehicleType << "\n";
            cout << " Masuk     : " << ctime(&v.entryTime);
            cout << " Keluar    : " << (v.exitTime ? ctime(&v.exitTime) : "Masih parkir\n");
            garis();
        }
    }
    cout << " Tekan enter untuk kembali...";
    cin.ignore();
    cin.get();
}

int main() {
    int choice;
    do {
        header("SISTEM PARKIR DIGITAL");
        cout << " 1. Transaksi Kendaraan\n";
        cout << " 2. Manajemen Jenis Kendaraan\n";
        cout << " 3. Laporan Transaksi\n";
        cout << " 4. Keluar\n";
        garis();
        cout << " Pilih menu : ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            header("KESALAHAN INPUT");
            cout << " Harap masukkan angka!\n";
            garis();
            continue;
        }

        switch (choice) {
            case 1: {
                header("MENU TRANSAKSI");
                cout << " 1. Kendaraan Masuk\n";
                cout << " 2. Kendaraan Keluar\n";
                garis();
                cout << " Pilih transaksi : ";
                int subChoice;
                cin >> subChoice;
                
                if (subChoice == 1) transactionEntry();
                else if (subChoice == 2) transactionExit();
                else cout << " Pilihan tidak valid!\n";
                break;
            }
            case 2: manageVehicleTypes(); break;
            case 3: listTransactions(); break;
            case 4: cout << "Aplikasi ditutup\n"; break;
            default: cout << " Pilihan tidak valid!\n";
        }
    } while (choice != 4);

    return 0;
}