// =====================================================================
// SOAL 1 (ARRAY 1 DIMENSI)
// Fusi Sensor Jarak Ultrasonik + Inframerah pada Robot AMR
// Nama : Rahma Keisya Widianto Putri  NIM : 26/584857/PA/24551  Kelas : EL A
// Kompilasi: g++ -std=c++17 -Wall -Wextra NIM_Nama_Soal1.cpp -o soal1
// =====================================================================
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
using namespace std;

int main() {
    const int N = 18;
    int us[N] = {83, 70, 59, 45, 34, 22, 0, 231, 0, 215, 0, 23, 82, 71, 58, 0, 34, 22};
    int ir[N] = {0, 71, 57, 47, 35, 23, 0, 69, 58, 46, 33, 21, 0, 95, 0, 0, 88, 0};

    // T1: konstanta, toleransi, ambang keputusan
    const int min_us = 2;
    const int max_us = 400;
    const int max_ir = 80;
    const int min_ir = 10;
    const int toleransi = 10;
    const float batas_berhenti = 25.0f;
    const float batas_pelan = 50.0f;

    // T1: array hasil
    bool validUS[N];
    bool validIR[N];
    float fusi[N];
    string metode[N];
    string keputusan[N];

    // penghitung
    int jumlahus = 0;
    int jumlahir = 0;
    int jumlahfusi = 0;
    int jumlahkonflik = 0;
    int jumlahtanpa = 0;
    int idxkonflik[N];
    int idxtanpa[N];

    // T2 + T3: validasi dan fusi
    for (int i = 0; i < N; i++) {
        validUS[i] = us[i] >= min_us && us[i] <= max_us;
        if (validUS[i])
            jumlahus++;

        validIR[i] = ir[i] >= min_ir && ir[i] <= max_ir;
        if (validIR[i])
            jumlahir++;

        if (validUS[i] && validIR[i]) {
            int selisih = abs(us[i] - ir[i]);
            if (selisih <= toleransi) {
                fusi[i] = (us[i] + ir[i]) / 2.0f;
                metode[i] = "RATA2";
            } else {
                fusi[i] = (us[i] < ir[i]) ? us[i] : ir[i];
                metode[i] = "MIN";
                idxkonflik[jumlahkonflik++] = i;
            }
        } else if (validUS[i]) {
            fusi[i] = us[i];
            metode[i] = "US";
        } else if (validIR[i]) {
            fusi[i] = ir[i];
            metode[i] = "IR";
        } else {
            fusi[i] = -1;
            metode[i] = "-";
            idxtanpa[jumlahtanpa++] = i;
        }

        if (fusi[i] >= 0)
            jumlahfusi++;
    }

    // T4: keputusan gerak
    int jalan = 0, pelan = 0, berhenti = 0;
    int pertamaberhenti = -1;

    for (int i = 0; i < N; i++) {
        if (fusi[i] < 0) {
            keputusan[i] = "PELAN";
            pelan++;
        } else if (fusi[i] < batas_berhenti) {
            keputusan[i] = "BERHENTI";
            berhenti++;
            if (pertamaberhenti == -1)
                pertamaberhenti = i;
        } else if (fusi[i] < batas_pelan) {
            keputusan[i] = "PELAN";
            pelan++;
        } else {
            keputusan[i] = "JALAN";
            jalan++;
        }
    }

    // T4: tabel 18 baris
    cout << fixed << setprecision(1);
    cout << "==================================================================\n";
    cout << "  FUSI SENSOR JARAK - ROBOT PENGANTAR OBAT (" << N << " sampel)\n";
    cout << "==================================================================\n";
    cout << " No | US(cm) | IR(cm) | Valid | Fusi(cm) | Metode | Keputusan\n";
    cout << "----+--------+--------+-------+----------+--------+----------\n";
    for (int i = 0; i < N; i++) {
        string valid = "";
        valid += validUS[i] ? 'U' : '-';
        valid += validIR[i] ? 'I' : '-';

        cout << setw(3) << i << " |"
             << setw(7) << us[i] << " |"
             << setw(7) << ir[i] << " |"
             << "  " << valid << "   |";
        if (fusi[i] >= 0) cout << setw(9) << fusi[i];
        else              cout << setw(9) << "-";
        cout << " | " << left << setw(6) << metode[i] << right
             << " | " << keputusan[i] << "\n";
    }
    cout << "----+--------+--------+-------+----------+--------+----------\n\n";

    // T5: ringkasan
    int idxmin = -1;
    for (int i = 0; i < N; i++) {
        if (fusi[i] >= 0 && (idxmin == -1 || fusi[i] < fusi[idxmin]))
            idxmin = i;
    }

    cout << "RINGKASAN\n";
    cout << "Data valid ultrasonik : " << jumlahus << " dari " << N
         << " (" << 100.0 * jumlahus / N << " %)\n";
    cout << "Data valid inframerah : " << jumlahir << " dari " << N
         << " (" << 100.0 * jumlahir / N << " %)\n";
    cout << "Data fusi tersedia    : " << jumlahfusi << " dari " << N
         << " (" << 100.0 * jumlahfusi / N << " %)\n";

    cout << "Konflik antarsensor   : " << jumlahkonflik << " sampel ->";
    for (int k = 0; k < jumlahkonflik; k++) cout << " " << idxkonflik[k];
    cout << "\n";

    cout << "Tanpa data valid      : " << jumlahtanpa << " sampel ->";
    for (int k = 0; k < jumlahtanpa; k++) cout << " " << idxtanpa[k];
    cout << "\n";

    if (idxmin != -1)
        cout << "Jarak fusi minimum    : " << fusi[idxmin]
             << " cm (sampel " << idxmin << ")\n";
    else
        cout << "Jarak fusi minimum    : -1 (tidak ada data)\n";

    cout << "Jumlah keputusan      : JALAN " << jalan
         << " | PELAN " << pelan
         << " | BERHENTI " << berhenti << "\n";

    if (pertamaberhenti != -1)
        cout << "BERHENTI pertama      : sampel " << pertamaberhenti << "\n";
    else
        cout << "BERHENTI pertama      : tidak ada\n";
    cout << "==================================================================\n\n";

    // T6 (Bonus): grafik batang ASCII
    cout << "GRAFIK JARAK FUSI (1 '#' = 5 cm)\n";
    for (int i = 0; i < N; i++) {
        cout << setw(3) << i << " | ";
        if (fusi[i] < 0) {
            cout << "? (tidak ada data)\n";
        } else {
            int panjang = (int)(fusi[i] / 5.0f);
            for (int k = 0; k < panjang; k++) cout << '#';
            cout << " " << fusi[i] << "\n";
        }
    }

    return 0;
}
