#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
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
    char status[20];
} Transaksi;

typedef struct {
    char nama[50];
    char password[50];
    char lokasi[100];
    int saldo;
    int isMember;
} AkunPembeli;

int loginAdmin() {
    char user[50], pass[50];
    const char adminUser[] = "admin";
    const char adminPass[] = "mart123";

    for (int i = 1; i <= 3; i++) {
        printf("=== Login Admin (%d/3)\n", i);
        printf("Username: ");
        scanf(" %[^\n]", user);
        printf("Password: ");
        scanf(" %[^\n]", pass);

        if (strcmp(user, adminUser) == 0 && strcmp(pass, adminPass) == 0) {
            printf("Login berhasil.\n\n");
            return 1;
        } else {
            printf("Username atau password salah.\n");
        }
    }
    printf("Gagal login 3 kali.\n");
    return 0;
}
void bubbleSortProduk(Produk arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (strcmp(arr[j].namaProduk, arr[j+1].namaProduk) > 0) {
                Produk tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}
void lihatProduk() {
    Produk list[MAX];
    int n = 0;
    FILE *fp = fopen("produk.txt", "r");
    if (fp == NULL) {
        printf("Tidak ada produk.\n");
        return;
    }
    while (fscanf(fp, "%[^;];%d;%d\n", list[n].namaProduk, &list[n].harga, &list[n].stok) == 3) {
        n++;
    }
    fclose(fp);
    bubbleSortProduk(list, n);

    printf("Daftar Produk\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s - Harga: %d - Stok: %d\n", i+1, list[i].namaProduk, list[i].harga, list[i].stok);
    }
}
void tambahProduk() {
    Produk p;
    printf("Tambah Produk Baru\n");
    printf("Nama Produk: ");
    scanf(" %[^\n]", p.namaProduk);
    printf("Harga: ");
    scanf("%d", &p.harga);
    printf("Stok: ");
    scanf("%d", &p.stok);

    FILE *fp = fopen("produk.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "%s;%d;%d\n", p.namaProduk, p.harga, p.stok);
        fclose(fp);
        printf("Produk berhasil ditambahkan.\n");
    } else {
        printf("Gagal menyimpan produk.\n");
    }
}
void updateStok() {
    Produk list[MAX];
    int n = 0;
    FILE *fp = fopen("produk.txt", "r");
    if (fp == NULL) {
        printf("File produk tidak ditemukan.\n");
        return;
    }
    while (fscanf(fp, "%[^;];%d;%d\n", list[n].namaProduk, &list[n].harga, &list[n].stok) == 3) {
        n++;
    }
    fclose(fp);

    char cari[50];
    printf("Masukkan nama produk yang ingin diupdate stoknya: ");
    scanf(" %[^\n]", cari);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(list[i].namaProduk, cari) == 0) {
            printf("Stok saat ini: %d\n", list[i].stok);
            printf("Masukkan stok baru: ");
            scanf("%d", &list[i].stok);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Produk tidak ditemukan.\n");
        return;
    }
    fp = fopen("produk.txt", "w");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s;%d;%d\n", list[i].namaProduk, list[i].harga, list[i].stok);
    }
    fclose(fp);
    printf("Stok berhasil diperbarui.\n");
}
void lihatPendapatan() {
    FILE *fp = fopen("riwayat_pembelian.txt", "r");
    Transaksi t;
    int total = 0;
    while (fscanf(fp, "%[^;];%[^;];%d;%d;%d;%[^\n]\n",
                  t.namaPembeli, t.namaProduk, &t.harga, &t.jumlah, &t.total, t.status) == 6) {
        total += t.total;
    }
    fclose(fp);
    printf("Total Pendapatan: Rp %d\n", total);
}
void lihatUlasan() {
    FILE *fp = fopen("ulasan.txt", "r");
    char ulasan[200];
    printf("Ulasan Pembeli\n");
    while (fgets(ulasan, sizeof(ulasan), fp)) {
        printf("- %s", ulasan);
    }
    fclose(fp);
}
void hapusAkun() {
    char nama[50];
    printf("Masukkan nama akun pembeli yang akan dihapus: ");
    scanf(" %[^\n]", nama);

    FILE *fp = fopen("akun_pembeli.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    AkunPembeli akun;
    int ditemukan = 0;

    while (fscanf(fp, "%[^;];%[^;];%[^;];%d;%d\n",
                  akun.nama, akun.password, akun.lokasi, &akun.saldo, &akun.isMember) == 5) {
        if (strcmp(akun.nama, nama) != 0) {
            fprintf(temp, "%s;%s;%s;%d;%d\n",
                    akun.nama, akun.password, akun.lokasi, akun.saldo, akun.isMember);
        } else {
            ditemukan = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("akun_pembeli.txt");
    rename("temp.txt", "akun_pembeli.txt");
    if (ditemukan)
        printf("Akun berhasil dihapus.\n");
    else
        printf("Akun tidak ditemukan.\n");
}
void lihatPesananDiterima() {
    FILE *fp = fopen("pesanan_diterima.txt", "r");
    Transaksi t;
    printf("Pesanan Diterima\n");
    while (fscanf(fp, "%[^;];%[^;];%d;%d;%d;%[^\n]\n",
                  t.namaPembeli, t.namaProduk, &t.harga, &t.jumlah, &t.total, t.status) == 6) {
        printf("%s menerima %d x %s - Total: %d\n", t.namaPembeli, t.jumlah, t.namaProduk, t.total);
    }
    fclose(fp);
}
void lihatRiwayatPembelian() {
    FILE *fp = fopen("riwayat_pembelian.txt", "r");
    Transaksi t;
    printf("Riwayat Pemesanan Pembeli\n");
    while (fscanf(fp, "%[^;];%[^;];%d;%d;%d;%[^\n]\n",
                  t.namaPembeli, t.namaProduk, &t.harga, &t.jumlah, &t.total, t.status) == 6) {
        printf("%s beli %d x %s - Status: %s\n", t.namaPembeli, t.jumlah, t.namaProduk, t.status);
    }
    fclose(fp);
}
void menuAdmin() {
    int pilih;
    do {
        printf("\nMENU PENJUAL\n");
        printf("1. Cek Produk yang Dijual\n");
        printf("2. Tambah Produk\n");
        printf("3. Update Stok Produk\n");
        printf("4. Lihat Pendapatan\n");
        printf("5. Lihat Ulasan Pembeli\n");
        printf("6. Hapus Akun Pembeli\n");
        printf("7. Lihat Pesanan yang Sudah Sampai\n");
        printf("8. Lihat Riwayat Pemesanan Pembeli\n");
        printf("9. Logout\n");
        printf("Pilihan: ");
        scanf("%d", &pilih);

        switch (pilih) {
            case 1: lihatProduk(); break;
            case 2: tambahProduk(); break;
            case 3: updateStok(); break;
            case 4: lihatPendapatan(); break;
            case 5: lihatUlasan(); break;
            case 6: hapusAkun(); break;
            case 7: lihatPesananDiterima(); break;
            case 8: lihatRiwayatPembelian(); break;
            case 9: printf("Logout berhasil.\n"); break;
            default: printf("Pilihan tidak valid.\n");
        }
    } while (pilih != 9);
}
int main() {
    printf("D'MART APLIKASI PENJUAL\n");
    if (loginAdmin()) {
        menuAdmin();
    }
    return 0;
}
