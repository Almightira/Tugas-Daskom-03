#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// ==== Struktur Data ====
typedef struct {
    char nama[50];
    char password[50];
    char lokasi[100];
    int saldo;
    int isMember;
} AkunPembeli;

typedef struct {
    char namaProduk[50];
    int harga;
    int stok;
} Produk;

typedef struct {
    char namaPembeli[50];
    char namaProduk[50];
    int harga;
    int jumlah;
    int total;
    char status[20]; // pending / diterima
} Transaksi;

// ==== Fungsi Akun ====
void simpanAkun(AkunPembeli akun) {
    FILE *fp = fopen("akun_pembeli.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "%s;%s;%s;%d;%d\n", akun.nama, akun.password, akun.lokasi, akun.saldo, akun.isMember);
        fclose(fp);
    } else {
        printf("Gagal membuka file untuk menyimpan akun.\n");
    }
}

int bacaSemuaAkun(AkunPembeli akunList[], int max) {
    FILE *fp = fopen("akun_pembeli.txt", "r");
    int count = 0;
    if (fp != NULL) {
        while (fscanf(fp, "%[^;];%[^;];%[^;];%d;%d\n",
                      akunList[count].nama,
                      akunList[count].password,
                      akunList[count].lokasi,
                      &akunList[count].saldo,
                      &akunList[count].isMember) == 5) {
            count++;
            if (count >= max) break;
        }
        fclose(fp);
    }
    return count;
}

void updateSaldo(AkunPembeli akun) {
    AkunPembeli akunList[MAX];
    int total = bacaSemuaAkun(akunList, MAX);
    FILE *fp = fopen("akun_pembeli.txt", "w");

    for (int i = 0; i < total; i++) {
        if (strcmp(akun.nama, akunList[i].nama) == 0) {
            akunList[i].saldo = akun.saldo;
        }
        fprintf(fp, "%s;%s;%s;%d;%d\n",
                akunList[i].nama,
                akunList[i].password,
                akunList[i].lokasi,
                akunList[i].saldo,
                akunList[i].isMember);
    }

    fclose(fp);
}

void registrasi() {
    AkunPembeli akunBaru;

    printf("=== Registrasi Akun Pembeli ===\n");
    printf("Masukkan nama: ");
    scanf(" %[^\n]", akunBaru.nama);
    printf("Masukkan password: ");
    scanf(" %[^\n]", akunBaru.password);
    printf("Masukkan lokasi tempat tinggal: ");
    scanf(" %[^\n]", akunBaru.lokasi);
    printf("Menjadi member? (1 = Ya, 0 = Tidak): ");
    scanf("%d", &akunBaru.isMember);
    akunBaru.saldo = 0;

    simpanAkun(akunBaru);
    printf("Akun berhasil didaftarkan!\n\n");
}

int login(AkunPembeli *akunLogin) {
    char inputNama[50], inputPassword[50];
    AkunPembeli akunList[MAX];
    int totalAkun = bacaSemuaAkun(akunList, MAX);

    for (int attempt = 1; attempt <= 3; attempt++) {
        printf("=== Login Pembeli === (%d/3)\n", attempt);
        printf("Nama: ");
        scanf(" %[^\n]", inputNama);
        printf("Password: ");
        scanf(" %[^\n]", inputPassword);

        for (int i = 0; i < totalAkun; i++) {
            if (strcmp(inputNama, akunList[i].nama) == 0 &&
                strcmp(inputPassword, akunList[i].password) == 0) {
                *akunLogin = akunList[i];
                return 1;
            }
        }
        printf("Login gagal. Coba lagi.\n\n");
    }
    return 0;
}

// ==== Fungsi Produk ====
int bacaProduk(Produk daftar[], int max) {
    FILE *fp = fopen("produk.txt", "r");
    int count = 0;
    if (fp != NULL) {
        while (fscanf(fp, "%[^;];%d;%d\n",
                      daftar[count].namaProduk,
                      &daftar[count].harga,
                      &daftar[count].stok) == 3) {
            count++;
            if (count >= max) break;
        }
        fclose(fp);
    }
    return count;
}

void bubbleSortProduk(Produk arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j].namaProduk, arr[j+1].namaProduk) > 0) {
                Produk temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void tampilkanProduk() {
    Produk daftar[MAX];
    int jumlah = bacaProduk(daftar, MAX);

    if (jumlah == 0) {
        printf("Belum ada produk tersedia.\n");
        return;
    }

    bubbleSortProduk(daftar, jumlah);
    printf("=== Daftar Produk ===\n");
    for (int i = 0; i < jumlah; i++) {
        printf("%d. %s - Harga: %d - Stok: %d\n",
               i + 1, daftar[i].namaProduk, daftar[i].harga, daftar[i].stok);
    }
}

int cariProduk(Produk daftar[], int jumlah, char namaDicari[]) {
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(daftar[i].namaProduk, namaDicari) == 0) {
            return i;
        }
    }
    return -1;
}

// ==== Fungsi Transaksi ====
void simpanTransaksi(Transaksi trx) {
    FILE *fp = fopen("riwayat_pembelian.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "%s;%s;%d;%d;%d;%s\n", trx.namaPembeli, trx.namaProduk,
                trx.harga, trx.jumlah, trx.total, trx.status);
        fclose(fp);
    }
}

void checkout(AkunPembeli *akun) {
    Produk daftar[MAX];
    int jumlah = bacaProduk(daftar, MAX);
    char namaCari[50];
    int indeks, jumlahBeli;

    if (jumlah == 0) {
        printf("Tidak ada produk tersedia.\n");
        return;
    }

    tampilkanProduk();
    printf("\nMasukkan nama produk yang ingin dibeli: ");
    scanf(" %[^\n]", namaCari);

    indeks = cariProduk(daftar, jumlah, namaCari);

    if (indeks == -1) {
        printf("Produk tidak ditemukan.\n");
        return;
    }

    printf("Masukkan jumlah yang ingin dibeli: ");
    scanf("%d", &jumlahBeli);

    if (jumlahBeli > daftar[indeks].stok) {
        printf("Stok tidak mencukupi.\n");
        return;
    }

    int totalHarga = jumlahBeli * daftar[indeks].harga;
    if (akun->saldo < totalHarga) {
        printf("Saldo tidak cukup.\n");
        return;
    }

    akun->saldo -= totalHarga;
    daftar[indeks].stok -= jumlahBeli;
    updateSaldo(*akun);

    Transaksi trx;
    strcpy(trx.namaPembeli, akun->nama);
    strcpy(trx.namaProduk, daftar[indeks].namaProduk);
    trx.harga = daftar[indeks].harga;
    trx.jumlah = jumlahBeli;
    trx.total = totalHarga;
    strcpy(trx.status, "pending");

    simpanTransaksi(trx);

    FILE *fp = fopen("produk.txt", "w");
    for (int i = 0; i < jumlah; i++) {
        fprintf(fp, "%s;%d;%d\n", daftar[i].namaProduk, daftar[i].harga, daftar[i].stok);
    }
    fclose(fp);

    printf("Pembelian berhasil. Sisa saldo: %d\n", akun->saldo);
}

// ==== Fungsi Lain ====
void konfirmasiPesanan(const char *namaPembeli) {
    FILE *fp = fopen("riwayat_pembelian.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    FILE *konfirmasi = fopen("pesanan_diterima.txt", "a");
    Transaksi trx;
    int found = 0;

    while (fscanf(fp, "%[^;];%[^;];%d;%d;%d;%[^\n]\n", trx.namaPembeli, trx.namaProduk,
                  &trx.harga, &trx.jumlah, &trx.total, trx.status) == 6) {
        if (strcmp(trx.namaPembeli, namaPembeli) == 0 && strcmp(trx.status, "pending") == 0) {
            found = 1;
            strcpy(trx.status, "diterima");
            fprintf(konfirmasi, "%s;%s;%d;%d;%d;%s\n", trx.namaPembeli, trx.namaProduk,
                    trx.harga, trx.jumlah, trx.total, trx.status);
        }
        fprintf(temp, "%s;%s;%d;%d;%d;%s\n", trx.namaPembeli, trx.namaProduk,
                trx.harga, trx.jumlah, trx.total, trx.status);
    }

    fclose(fp);
    fclose(temp);
    fclose(konfirmasi);
    remove("riwayat_pembelian.txt");
    rename("temp.txt", "riwayat_pembelian.txt");

    if (found)
        printf("Pesanan telah dikonfirmasi.\n");
    else
        printf("Tidak ada pesanan untuk dikonfirmasi.\n");
}

void topUpSaldo(AkunPembeli *akun) {
    int jumlah;
    printf("Saldo saat ini: %d\n", akun->saldo);
    printf("Masukkan jumlah top-up: ");
    scanf("%d", &jumlah);

    akun->saldo += jumlah;
    updateSaldo(*akun);
    printf("Top-up berhasil. Saldo baru: %d\n", akun->saldo);
}

void tulisUlasan(const char *namaPembeli) {
    char ulasan[200];
    printf("=== Beri Ulasan ===\n");
    printf("Masukkan ulasan Anda: ");
    scanf(" %[^\n]", ulasan);

    FILE *fp = fopen("ulasan.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "%s: %s\n", namaPembeli, ulasan);
        fclose(fp);
        printf("Ulasan berhasil disimpan.\n");
    } else {
        printf("Gagal menyimpan ulasan.\n");
    }
}

// ==== Menu Pembeli ====
void menuPembeli(AkunPembeli *akunLogin) {
    int pilih;
    do {
        printf("\n=== Menu Pembeli ===\n");
        printf("1. Lihat Produk\n");
        printf("2. Top-Up Saldo\n");
        printf("3. Lihat Saldo\n");
        printf("4. Beli Produk\n");
        printf("5. Konfirmasi Pesanan Diterima\n");
        printf("6. Tulis Ulasan\n");
        printf("7. Logout\n");
        printf("Pilihan: ");
        scanf("%d", &pilih);

        switch (pilih) {
            case 1: tampilkanProduk(); break;
            case 2: topUpSaldo(akunLogin); break;
            case 3: printf("Saldo: %d\n", akunLogin->saldo); break;
            case 4: checkout(akunLogin); break;
            case 5: konfirmasiPesanan(akunLogin->nama); break;
            case 6: tulisUlasan(akunLogin->nama); break;
            case 7: printf("Logout...\n"); break;
            default: printf("Pilihan tidak valid.\n");
        }
    } while (pilih != 7);
}

// ==== Main Program ====
int main() {
    AkunPembeli akunLogin;
    int pilih;

    while (1) {
        printf("\n=== D'Mart - Menu Utama ===\n");
        printf("1. Registrasi\n");
        printf("2. Login\n");
        printf("3. Keluar\n");
        printf("Pilihan: ");
        scanf("%d", &pilih);

        switch (pilih) {
            case 1: registrasi(); break;
            case 2:
                if (login(&akunLogin)) {
                    printf("Selamat datang, %s!\n", akunLogin.nama);
                    menuPembeli(&akunLogin);
                } else {
                    printf("Gagal login.\n");
                }
                break;
            case 3:
                printf("Terima kasih telah menggunakan D'Mart.\n");
                return 0;
            default:
                printf("Pilihan tidak valid!\n");
        }
    }
}
