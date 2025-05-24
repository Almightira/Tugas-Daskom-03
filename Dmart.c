#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
 
struct produk {
    char namaProduk[50];
    int hargaProduk;
    int stokProduk;
    int idProduk;
};

struct akun_pembeli {
    char namaPembeli[50];
    char passwordPembeli[50];
    char lokasiPembeli[100];
    int saldoPembeli;
    bool isMember;
    int idPembeli;
};

struct pesanan {
    int idPesanan;
    int idPembeli;
    int idProduk;
    char namaProdukPesanan[50];
    int jumlahPesanan;
    int totalHargaPesanan;
    char statusPesanan[30];
};

struct ulasan {
    int idUlasan;
    int idPembeli;
    int idProduk;
    char isiUlasan[200];
};

// --- Fungsi Bantuan ---
void clearScreen() {
    system("cls");
}

void pressEnterToContinue() {
    printf("\nTekan Enter...");
    fflush(stdin);
    getchar();
}

int getNextId(const char* namaFileCounter) {
    FILE* fileCounterData;
    int id = 0;
    fileCounterData = fopen(namaFileCounter, "rb");
    if (fileCounterData != NULL) {
        fread(&id, sizeof(int), 1, fileCounterData);
        fclose(fileCounterData);
    }
    id++;
    fileCounterData = fopen(namaFileCounter, "wb");
    if (fileCounterData != NULL) {
        fwrite(&id, sizeof(int), 1, fileCounterData);
        fclose(fileCounterData);
    } else {
        printf("Error: Gagal update file counter ID (%s)!\n", namaFileCounter);
    }
    return id;
}


// penjual
void menuPenjual();
void loginPenjual();
void produkPenjual();
void lihatSemuaProdukPenjual();
void tambahProdukPenjual();
void updateProdukPenjual();
void lihatPendapatanPenjual();
void lihatRiwayatPemesananPembeliOlehPenjual();
void hapusAkunPembeliOlehPenjual();
void lihatUlasanOlehPenjual();
void lihatPesananSelesaiOlehPenjual();

// pembeli
void menuPembeli();
void registrasiAkunPembeli();
void loginPembeli(struct akun_pembeli *akunPembeliLogin, bool *statusLoginBerhasil);
void beliProdukOlehPembeli(struct akun_pembeli *akunPembeliSaatIni);
void checkoutPembeli(struct akun_pembeli *akunPembeliSaatIni, struct produk keranjangBeli[], int jumlahItemDiKeranjang, int kuantitasDiKeranjang[]);
void topUpSaldoOlehPembeli(struct akun_pembeli *akunPembeliSaatIni);
void lihatRiwayatPembelianOlehPembeli(int idPembeli);
void konfirmasiPesananDiterimaOlehPembeli(int idPembeli);
void tulisUlasanOlehPembeli(int idPembeli);

// --- Fungsi Sorting & Searching ---
void sortProdukByName(struct produk arrayProduk[], int jumlahProduk) {
    int i, j;
    struct produk tempProduk;
    if (jumlahProduk <= 1) return;
    for (i = 0; i < jumlahProduk - 1; i++) {
        for (j = 0; j < jumlahProduk - i - 1; j++) {
            // Perbandingan string sekarang case-sensitive
            if (strcmp(arrayProduk[j].namaProduk, arrayProduk[j + 1].namaProduk) > 0) {
                tempProduk = arrayProduk[j];
                arrayProduk[j] = arrayProduk[j + 1];
                arrayProduk[j + 1] = tempProduk;
            }
        }
    }
}

int searchProdukById(int idProdukCari, struct produk arrayProduk[], int jumlahProduk, int* indexDitemukan) {
    int i;
    for (i = 0; i < jumlahProduk; i++) {
        if (arrayProduk[i].idProduk == idProdukCari) {
            if (indexDitemukan != NULL) *indexDitemukan = i;
            return 1;
        }
    }
    return 0;
}

// --- Fungsi Utama ---
int main() {
    int pilihanUtama;
    do {
        clearScreen();
        printf("========================\n");
        printf("   Selamat Datang DMart   \n");
        printf("========================\n");
        printf("Masuk sebagai:\n1. Penjual\n2. Pembeli\n3. Keluar\nPilihan: ");
        if (scanf("%d", &pilihanUtama) != 1) pilihanUtama = 0;
        fflush(stdin);

        switch (pilihanUtama) {
            case 1: loginPenjual(); break;
            case 2: menuPembeli(); break;
            case 3: printf("\nTerima kasih!\n"); break;
            default: printf("Pilihan salah.\n"); pressEnterToContinue();
        }
    } while (pilihanUtama != 3);
    return 0;
}

// --- Implementasi Fungsi Penjual ---
void loginPenjual() {
    char inputUsername[50], inputPassword[50];
    int percobaanLogin = 0; const int maks_percobaan = 3;
    const char* username_admin_valid = "admin"; const char* password_admin_valid = "admin123";
    do {
        clearScreen(); printf("--- Login Penjual ---\nUsername: ");
        fgets(inputUsername, sizeof(inputUsername), stdin); inputUsername[strcspn(inputUsername, "\n")] = 0;
        printf("Password: "); fgets(inputPassword, sizeof(inputPassword), stdin); inputPassword[strcspn(inputPassword, "\n")] = 0;
        // Perbandingan username dan password akan case-sensitive
        if (strcmp(inputUsername, username_admin_valid) == 0 && strcmp(inputPassword, password_admin_valid) == 0) {
            printf("Login berhasil!\n"); pressEnterToContinue(); menuPenjual(); return;
        } else { percobaanLogin++; printf("Username/Password salah. Sisa coba: %d\n", maks_percobaan - percobaanLogin); pressEnterToContinue(); }
    } while (percobaanLogin < maks_percobaan);
    printf("Gagal login %d kali.\n", maks_percobaan); pressEnterToContinue();
}

void menuPenjual() {
    int pilihan;
    do {
        clearScreen();
        printf("--- Menu Penjual ---\n1. Produk\n2. Pendapatan\n3. Riwayat Pemesanan\n4. Hapus Akun Pembeli\n5. Lihat Ulasan\n6. Pesanan Sampai\n7. Logout\nPilihan: ");
        if (scanf("%d", &pilihan) != 1) pilihan = 0; fflush(stdin);
        switch (pilihan) {
            case 1: produkPenjual(); break;
            case 2: lihatPendapatanPenjual(); break;
            case 3: lihatRiwayatPemesananPembeliOlehPenjual(); break;
            case 4: hapusAkunPembeliOlehPenjual(); break;
            case 5: lihatUlasanOlehPenjual(); break;
            case 6: lihatPesananSelesaiOlehPenjual(); break;
            case 7: printf("Logout...\n"); pressEnterToContinue(); return;
            default: printf("Pilihan salah.\n"); pressEnterToContinue();
        }
    } while (true);
}

void produkPenjual() {
    int pilihan;
    do {
        clearScreen();
        printf("--- Kelola Produk ---\n1. Lihat Produk\n2. Tambah Produk\n3. Update Produk\n4. Kembali\nPilihan: ");
        if (scanf("%d", &pilihan) != 1) pilihan = 0; fflush(stdin);
        switch (pilihan) {
            case 1: lihatSemuaProdukPenjual(); break;
            case 2: tambahProdukPenjual(); break;
            case 3: updateProdukPenjual(); break;
            case 4: return;
            default: printf("Pilihan salah.\n"); pressEnterToContinue();
        }
    } while (true);
}

void lihatSemuaProdukPenjual() {
    FILE* fileProdukData; struct produk p; struct produk daftar[200]; int jumlah = 0, i;
    fileProdukData = fopen("produk.dat", "rb");
    if (fileProdukData == NULL) { printf("Belum ada produk.\n"); pressEnterToContinue(); return; }
    while (fread(&p, sizeof(struct produk), 1, fileProdukData) == 1 && jumlah < 200) daftar[jumlah++] = p;
    fclose(fileProdukData);
    clearScreen(); printf("--- Daftar Produk ---\n");
    if (jumlah == 0) printf("Kosong.\n");
    else {
        sortProdukByName(daftar, jumlah);
        printf("%-4s %-20s %-10s %s\n", "ID", "Nama", "Harga", "Stok");
        printf("----------------------------------------------\n");
        for (i = 0; i < jumlah; i++) printf("%-4d %-20s Rp%-8d %d\n", daftar[i].idProduk, daftar[i].namaProduk, daftar[i].hargaProduk, daftar[i].stokProduk);
    }
    pressEnterToContinue();
}

void tambahProdukPenjual() {
    FILE* fileProdukData; struct produk baru;
    baru.idProduk = getNextId("produkid.dat");
    clearScreen(); printf("--- Tambah Produk (ID: %d) ---\n", baru.idProduk);
    printf("Nama: "); fgets(baru.namaProduk, sizeof(baru.namaProduk), stdin); baru.namaProduk[strcspn(baru.namaProduk, "\n")] = 0;
    printf("Harga: Rp"); while(scanf("%d", &baru.hargaProduk)!=1 || baru.hargaProduk<0){ printf("Salah. Harga: Rp"); fflush(stdin); } fflush(stdin);
    printf("Stok: "); while(scanf("%d", &baru.stokProduk)!=1 || baru.stokProduk<0){ printf("Salah. Stok: "); fflush(stdin); } fflush(stdin);
    fileProdukData = fopen("produk.dat", "ab");
    if (fileProdukData == NULL) { printf("Error file.\n"); pressEnterToContinue(); return; }
    fwrite(&baru, sizeof(struct produk), 1, fileProdukData); fclose(fileProdukData);
    printf("Produk '%s' (ID: %d) ditambah.\n", baru.namaProduk, baru.idProduk); pressEnterToContinue();
}

void updateProdukPenjual() {
    FILE *asli, *sementara; struct produk p; int idUpdate; bool ketemu = false; char pilih;
    clearScreen(); printf("--- Update Produk ---\n"); lihatSemuaProdukPenjual();
    printf("\nID Produk update: "); if(scanf("%d", &idUpdate)!=1){fflush(stdin);printf("ID salah.\n");pressEnterToContinue();return;} fflush(stdin);
    asli = fopen("produk.dat", "rb"); if (asli == NULL) { printf("File produk.dat error.\n"); pressEnterToContinue(); return; }
    sementara = fopen("temp_produk.dat", "wb"); if (sementara == NULL) { printf("Error file sementara.\n"); fclose(asli); pressEnterToContinue(); return; }
    while (fread(&p, sizeof(struct produk), 1, asli) == 1) {
        if (p.idProduk == idUpdate) {
            ketemu = true;
            printf("\nProduk Ditemukan: ID:%d, Nama:%s\nUpdate: (N)ama, (H)arga, (S)tok, (B)atal? ", p.idProduk, p.namaProduk); scanf(" %c", &pilih); fflush(stdin);
            if (pilih=='N'||pilih=='n'){printf("Nama baru: ");fgets(p.namaProduk,sizeof(p.namaProduk),stdin);p.namaProduk[strcspn(p.namaProduk,"\n")]=0;}
            else if (pilih=='H'||pilih=='h'){printf("Harga baru: Rp");while(scanf("%d",&p.hargaProduk)!=1||p.hargaProduk<0){printf("Salah. Harga: Rp");fflush(stdin);};fflush(stdin);}
            else if (pilih=='S'||pilih=='s'){printf("Stok baru: ");while(scanf("%d",&p.stokProduk)!=1||p.stokProduk<0){printf("Salah. Stok: ");fflush(stdin);};fflush(stdin);}
            else printf("Update batal.\n");
        }
        fwrite(&p, sizeof(struct produk), 1, sementara);
    }
    fclose(asli); fclose(sementara);
    if (ketemu) { remove("produk.dat"); rename("temp_produk.dat", "produk.dat"); printf("Produk ID %d diproses.\n", idUpdate); }
    else { remove("temp_produk.dat"); printf("Produk ID %d tidak ada.\n", idUpdate); }
    pressEnterToContinue();
}

void lihatPendapatanPenjual() {
    FILE* filePemesananData; struct pesanan psn; long total = 0; bool ada = false;
    filePemesananData = fopen("pesanan.dat", "rb"); if (filePemesananData == NULL) { printf("Belum ada pesanan.\n"); pressEnterToContinue(); return; }
    clearScreen(); printf("--- Pendapatan ---\n");
    while (fread(&psn, sizeof(struct pesanan), 1, filePemesananData) == 1) if (strcmp(psn.statusPesanan, "Diterima") == 0 || strcmp(psn.statusPesanan, "Dibayar") == 0) { total += psn.totalHargaPesanan; ada = true; }
    fclose(filePemesananData); if (ada) printf("Total Pendapatan: Rp%ld\n", total); else printf("Belum ada pendapatan.\n");
    pressEnterToContinue();
}

void lihatRiwayatPemesananPembeliOlehPenjual() {
    FILE* filePemesananData; struct pesanan psn; bool ada = false;
    filePemesananData = fopen("pesanan.dat", "rb"); if (filePemesananData == NULL) { printf("Belum ada pesanan.\n"); pressEnterToContinue(); return; }
    clearScreen(); printf("--- Riwayat Semua Pesanan ---\n");
    printf("%-7s %-10s %-10s %-20s %-3s %-10s %s\n", "IDPsn", "IDPbeli", "IDProd", "Produk", "Jml", "Total", "Status");
    printf("-----------------------------------------------------------------------------------\n");
    while (fread(&psn, sizeof(struct pesanan), 1, filePemesananData) == 1) { ada = true; printf("%-7d %-10d %-10d %-20s %-3d Rp%-8d %s\n", psn.idPesanan, psn.idPembeli, psn.idProduk, psn.namaProdukPesanan, psn.jumlahPesanan, psn.totalHargaPesanan, psn.statusPesanan); }
    fclose(filePemesananData); if (!ada) printf("Kosong.\n");
    pressEnterToContinue();
}

void hapusAkunPembeliOlehPenjual() {
    FILE *akunPembeliFile1, *akunPembeliFile2; struct akun_pembeli akun; int idHapus; bool ketemu = false; char konfirm; int jumlahAkun = 0;
    clearScreen(); printf("--- Hapus Akun Pembeli ---\nDaftar Akun Pembeli:\n%-5s %-20s\n---------------------------\n", "ID", "Nama Pembeli");
    akunPembeliFile1 = fopen("pembeli.dat", "rb");
    if (akunPembeliFile1 != NULL) { while (fread(&akun, sizeof(struct akun_pembeli), 1, akunPembeliFile1) == 1) { printf("%-5d %-20s\n", akun.idPembeli, akun.namaPembeli); jumlahAkun++; } fclose(akunPembeliFile1); }
    else printf("Belum ada akun.\n");
    if (jumlahAkun == 0) { pressEnterToContinue(); return; } printf("---------------------------\n");
    printf("ID Pembeli yg dihapus: "); if(scanf("%d", &idHapus)!=1){fflush(stdin);printf("ID Salah.\n");pressEnterToContinue();return;} fflush(stdin);
    akunPembeliFile1 = fopen("pembeli.dat", "rb"); if (akunPembeliFile1 == NULL) { printf("File pembeli.dat error.\n"); pressEnterToContinue(); return; }
    akunPembeliFile2 = fopen("akun_pembeli_temp.dat", "wb"); if (akunPembeliFile2 == NULL) { printf("Error file sementara.\n"); fclose(akunPembeliFile1); pressEnterToContinue(); return; }
    while (fread(&akun, sizeof(struct akun_pembeli), 1, akunPembeliFile1) == 1) {
        if (akun.idPembeli == idHapus) {
            ketemu = true; printf("\nHapus '%s' (ID:%d)? (y/n): ", akun.namaPembeli, akun.idPembeli); scanf(" %c", &konfirm); fflush(stdin);
            if (konfirm == 'y' || konfirm == 'Y') printf("Akun dihapus.\n"); else { printf("Batal hapus.\n"); fwrite(&akun, sizeof(struct akun_pembeli), 1, akunPembeliFile2); }
        } else fwrite(&akun, sizeof(struct akun_pembeli), 1, akunPembeliFile2);
    } fclose(akunPembeliFile1); fclose(akunPembeliFile2);
    if (ketemu) { remove("pembeli.dat"); rename("akun_pembeli_temp.dat", "pembeli.dat"); printf("Proses selesai.\n"); }
    else { remove("akun_pembeli_temp.dat"); printf("Akun ID %d tidak ada.\n", idHapus); }
    pressEnterToContinue();
}

void lihatUlasanOlehPenjual() {
    FILE* fileUlasanData; struct ulasan u; bool ada = false;
    fileUlasanData = fopen("ulasan.dat", "rb"); if (fileUlasanData == NULL) { printf("Belum ada ulasan.\n"); pressEnterToContinue(); return; }
    clearScreen(); printf("--- Ulasan Pembeli ---\n");
    printf("%-7s %-10s %-10s %s\n", "IDUlsn", "IDPbeli", "IDProduk", "Ulasan");
    printf("----------------------------------------------------------\n");
    while (fread(&u, sizeof(struct ulasan), 1, fileUlasanData) == 1) { ada = true; printf("%-7d %-10d ", u.idUlasan, u.idPembeli); if (u.idProduk == 0) printf("%-10s ", "Umum"); else printf("%-10d ", u.idProduk); printf("%s\n", u.isiUlasan); }
    fclose(fileUlasanData); if (!ada) printf("Kosong.\n");
    pressEnterToContinue();
}

void lihatPesananSelesaiOlehPenjual() {
    FILE* filePemesananData; struct pesanan psn; bool ada = false;
    filePemesananData = fopen("pesanan.dat", "rb"); if (filePemesananData == NULL) { printf("Belum ada pesanan.\n"); pressEnterToContinue(); return; }
    clearScreen(); printf("--- Pesanan Sudah Sampai (Diterima) ---\n");
    printf("%-7s %-10s %-20s %s\n", "IDPesn", "IDPbeli", "Produk", "Total");
    printf("---------------------------------------------------------\n");
    while (fread(&psn, sizeof(struct pesanan), 1, filePemesananData) == 1) if (strcmp(psn.statusPesanan, "Diterima") == 0) { ada = true; printf("%-7d %-10d %-20s Rp%d\n", psn.idPesanan, psn.idPembeli, psn.namaProdukPesanan, psn.totalHargaPesanan); }
    fclose(filePemesananData); if (!ada) printf("Belum ada pesanan diterima.\n");
    pressEnterToContinue();
}

// --- Implementasi Fungsi Pembeli ---
void menuPembeli() {
    int pilihan; struct akun_pembeli pembeliLogin; bool sudahLogin = false;
    do {
        clearScreen();
        if (!sudahLogin) {
            printf("--- Menu Pembeli ---\n1. Registrasi\n2. Login\n3. Kembali\nPilihan: ");
            if(scanf("%d", &pilihan)!=1) pilihan=0; fflush(stdin);
            if (pilihan==1) registrasiAkunPembeli(); else if (pilihan==2) loginPembeli(&pembeliLogin, &sudahLogin); else if (pilihan==3) return;
            else {printf("Pilihan salah.\n"); pressEnterToContinue();}
        } else {
            printf("--- Halo, %s! (ID: %d | Saldo: Rp%d) ---\n", pembeliLogin.namaPembeli, pembeliLogin.idPembeli, pembeliLogin.saldoPembeli);
            printf("1. Beli Produk\n2. Top Up Saldo\n3. Riwayat Pembelian\n4. Konfirmasi Pesanan\n5. Tulis Ulasan\n6. Logout\nPilihan: ");
            if(scanf("%d", &pilihan)!=1) pilihan=0; fflush(stdin);
            if (pilihan==1) { beliProdukOlehPembeli(&pembeliLogin); FILE* fa = fopen("pembeli.dat", "rb"); struct akun_pembeli tempA; if(fa){ while(fread(&tempA, sizeof(struct akun_pembeli),1,fa)==1) if(tempA.idPembeli == pembeliLogin.idPembeli) pembeliLogin.saldoPembeli = tempA.saldoPembeli; fclose(fa); }}
            else if (pilihan==2) topUpSaldoOlehPembeli(&pembeliLogin); else if (pilihan==3) lihatRiwayatPembelianOlehPembeli(pembeliLogin.idPembeli);
            else if (pilihan==4) konfirmasiPesananDiterimaOlehPembeli(pembeliLogin.idPembeli); else if (pilihan==5) tulisUlasanOlehPembeli(pembeliLogin.idPembeli);
            else if (pilihan==6) {sudahLogin=false; printf("Logout.\n"); pressEnterToContinue();} else {printf("Pilihan salah.\n"); pressEnterToContinue();}
        }
    } while (true);
}

void registrasiAkunPembeli() {
    FILE* fileAkunData; struct akun_pembeli baru; char member;
    baru.idPembeli = getNextId("pembeliid.dat");
    baru.saldoPembeli = 0;
    clearScreen(); printf("--- Registrasi Akun Pembeli (ID Anda: %d) ---\n", baru.idPembeli);
    printf("Nama Lengkap: "); fgets(baru.namaPembeli, sizeof(baru.namaPembeli), stdin); baru.namaPembeli[strcspn(baru.namaPembeli, "\n")] = 0;
    printf("Password: "); fgets(baru.passwordPembeli, sizeof(baru.passwordPembeli), stdin); baru.passwordPembeli[strcspn(baru.passwordPembeli, "\n")] = 0;
    printf("Lokasi: "); fgets(baru.lokasiPembeli, sizeof(baru.lokasiPembeli), stdin); baru.lokasiPembeli[strcspn(baru.lokasiPembeli, "\n")] = 0;
    printf("Jadi member (y/n)? "); scanf(" %c", &member); fflush(stdin); baru.isMember = (member=='y'||member=='Y');
    fileAkunData = fopen("pembeli.dat", "ab"); if(fileAkunData==NULL){printf("Error file.\n");pressEnterToContinue();return;}
    fwrite(&baru, sizeof(struct akun_pembeli), 1, fileAkunData); fclose(fileAkunData);
    printf("Registrasi sukses! ID Anda: %d. Silakan login.\n", baru.idPembeli); pressEnterToContinue();
}

void loginPembeli(struct akun_pembeli *akunLogin, bool *statusLogin) {
    char nama[50], pass[50]; FILE* fileAkunData; struct akun_pembeli cek; bool sukses = false;
    clearScreen(); printf("--- Login Pembeli ---\nNama: ");
    fgets(nama, sizeof(nama), stdin); nama[strcspn(nama, "\n")] = 0;
    printf("Password: "); fgets(pass, sizeof(pass), stdin); pass[strcspn(pass, "\n")] = 0;
    fileAkunData = fopen("pembeli.dat", "rb");
    if (fileAkunData != NULL) { while(fread(&cek, sizeof(struct akun_pembeli), 1, fileAkunData) == 1) if (strcmp(cek.namaPembeli, nama) == 0 && strcmp(cek.passwordPembeli, pass) == 0) { *akunLogin = cek; *statusLogin = true; sukses = true; printf("Login sukses, %s!\n", akunLogin->namaPembeli); break; } fclose(fileAkunData); }
    if (!sukses) { printf("Nama/Password salah.\n"); *statusLogin = false; }
    pressEnterToContinue();
}

void beliProdukOlehPembeli(struct akun_pembeli *pembeli) {
    FILE* fileProdukData; struct produk p; struct produk daftar[200]; struct produk keranjang[20]; int qtyKeranjang[20];
    int jmlProdukAda = 0, itemKeranjang = 0, qtyBeli, idxKetemu, idBeli; char lanjut;

    do {
        clearScreen(); printf("--- Beli Produk (Saldo: Rp%d) ---\n", pembeli->saldoPembeli);
        printf("%-4s %-20s %-10s %s\n", "ID", "Nama", "Harga", "Stok");
        printf("----------------------------------------------\n");
        jmlProdukAda = 0; fileProdukData = fopen("produk.dat", "rb");
        if (fileProdukData != NULL) { while (fread(&p, sizeof(struct produk), 1, fileProdukData) == 1 && jmlProdukAda < 200) if(p.stokProduk>0) daftar[jmlProdukAda++] = p; fclose(fileProdukData); }
        if (jmlProdukAda == 0) { printf("Produk habis.\n"); pressEnterToContinue(); return; }
        sortProdukByName(daftar, jmlProdukAda);
        for (int i = 0; i < jmlProdukAda; i++) printf("%-4d %-20s Rp%-8d %d\n", daftar[i].idProduk, daftar[i].namaProduk, daftar[i].hargaProduk, daftar[i].stokProduk);
        printf("----------------------------------------------\n");
        if (itemKeranjang > 0) { printf("Keranjang: "); for(int i=0; i<itemKeranjang; i++) printf("%s(%d) ", keranjang[i].namaProduk, qtyKeranjang[i]); printf("\n"); }

        printf("ID Produk (0 u/ checkout, -1 u/ batal): ");
        if(scanf("%d", &idBeli)!=1){fflush(stdin);idBeli=-2;} fflush(stdin);

        if (idBeli == 0) { if (itemKeranjang > 0) checkoutPembeli(pembeli, keranjang, itemKeranjang, qtyKeranjang); else printf("Keranjang kosong.\n"); break; }
        if (idBeli == -1) { printf("Pembelian dibatalkan.\n"); break; }
        if (idBeli < -1) {printf("Input ID salah.\n"); continue;}

        if (searchProdukById(idBeli, daftar, jmlProdukAda, &idxKetemu)) {
            printf("Jumlah '%s' (Stok: %d): ", daftar[idxKetemu].namaProduk, daftar[idxKetemu].stokProduk);
            if(scanf("%d", &qtyBeli)!=1 || qtyBeli<=0){fflush(stdin);printf("Jumlah salah.\n");continue;} fflush(stdin);
            bool adaDiKeranjang = false; int idxDiKeranjang = -1;
            for(int k=0; k < itemKeranjang; k++) if(keranjang[k].idProduk == daftar[idxKetemu].idProduk){adaDiKeranjang=true; idxDiKeranjang=k; break;}
            int stokAsli = daftar[idxKetemu].stokProduk; int qtyDiKeranjangSblm = adaDiKeranjang ? qtyKeranjang[idxDiKeranjang] : 0;
            if (qtyBeli > 0 && (qtyDiKeranjangSblm + qtyBeli) <= stokAsli) { if(adaDiKeranjang) qtyKeranjang[idxDiKeranjang] += qtyBeli; else if (itemKeranjang < 20) { keranjang[itemKeranjang]=daftar[idxKetemu]; qtyKeranjang[itemKeranjang]=qtyBeli; itemKeranjang++; } else printf("Keranjang penuh.\n"); printf("OK. '%s' x%d.\n", daftar[idxKetemu].namaProduk, qtyBeli); }
            else if (qtyBeli <= 0) printf("Jumlah salah.\n"); else printf("Stok '%s' gak cukup.\n", daftar[idxKetemu].namaProduk);
        } else printf("Produk ID %d tidak ada/habis.\n", idBeli);
        printf("Lanjut (y/n)? "); scanf(" %c", &lanjut); fflush(stdin); if (lanjut!='y' && lanjut!='Y') { if(itemKeranjang>0) checkoutPembeli(pembeli, keranjang, itemKeranjang, qtyKeranjang); break; }
    } while (true); pressEnterToContinue();
}

void checkoutPembeli(struct akun_pembeli *pembeli, struct produk keranjang[], int jmlItem, int qtyKeranjang[]) {
    clearScreen(); printf("--- Checkout ---\n"); if (jmlItem == 0) { printf("Keranjang kosong.\n"); pressEnterToContinue(); return; }
    printf("%-20s %-5s %s\n---------------------------------------\n", "Produk", "Qty", "Subtotal"); long totalBayar = 0;
    for (int i=0; i<jmlItem; i++) { long sub = (long)keranjang[i].hargaProduk * qtyKeranjang[i]; printf("%-20s %-5d Rp%ld\n", keranjang[i].namaProduk, qtyKeranjang[i], sub); totalBayar += sub; }
    printf("---------------------------------------\nTotal: Rp%ld | Saldo: Rp%d\n", totalBayar, pembeli->saldoPembeli);
    if (pembeli->saldoPembeli >= totalBayar) {
        char konfirm; printf("Bayar (y/n)? "); scanf(" %c", &konfirm); fflush(stdin);
        if (konfirm == 'y' || konfirm == 'Y') {
            pembeli->saldoPembeli -= totalBayar;
            FILE *fAkunAsli=fopen("pembeli.dat","rb"), *fAkunSementara=fopen("temp_akun.dat","wb"); struct akun_pembeli akun; bool sldUpd=false;
            if(fAkunAsli&&fAkunSementara){while(fread(&akun,sizeof(struct akun_pembeli),1,fAkunAsli)==1){if(akun.idPembeli==pembeli->idPembeli){akun.saldoPembeli=pembeli->saldoPembeli;sldUpd=true;}fwrite(&akun,sizeof(struct akun_pembeli),1,fAkunSementara);}fclose(fAkunAsli);fclose(fAkunSementara);if(sldUpd){remove("pembeli.dat");rename("temp_akun.dat","pembeli.dat");}else remove("temp_akun.dat");}
            for(int i=0;i<jmlItem;i++){FILE*fProdAsli=fopen("produk.dat","rb"),*fProdSementara=fopen("temp_prod.dat","wb");struct produk prd;bool stkUpd=false;if(fProdAsli&&fProdSementara){while(fread(&prd,sizeof(struct produk),1,fProdAsli)==1){if(prd.idProduk==keranjang[i].idProduk){prd.stokProduk-=qtyKeranjang[i];stkUpd=true;}fwrite(&prd,sizeof(struct produk),1,fProdSementara);}fclose(fProdAsli);fclose(fProdSementara);if(stkUpd){remove("produk.dat");rename("temp_prod.dat","produk.dat");}else remove("temp_prod.dat");}}
            FILE* fPesan=fopen("pesanan.dat","ab"); if(fPesan){for(int i=0;i<jmlItem;i++){struct pesanan psn; psn.idPesanan=getNextId("pesananid.dat"); psn.idPembeli=pembeli->idPembeli; psn.idProduk=keranjang[i].idProduk; strcpy(psn.namaProdukPesanan,keranjang[i].namaProduk);psn.jumlahPesanan=qtyKeranjang[i];psn.totalHargaPesanan=keranjang[i].hargaProduk*qtyKeranjang[i];strcpy(psn.statusPesanan,"Dibayar"); fwrite(&psn,sizeof(struct pesanan),1,fPesan);}fclose(fPesan);}
            printf("Bayar berhasil! Saldo sisa: Rp%d\n", pembeli->saldoPembeli);
        } else printf("Batal bayar.\n");
    } else printf("Saldo gak cukup.\n"); pressEnterToContinue();
}

void topUpSaldoOlehPembeli(struct akun_pembeli *pembeli) {
    int topup; clearScreen(); printf("--- Top Up (Saldo: Rp%d) ---\n", pembeli->saldoPembeli);
    printf("Jumlah Top Up: Rp"); if(scanf("%d", &topup)!=1 || topup<=0){fflush(stdin);printf("Jumlah salah.\n");pressEnterToContinue();return;} fflush(stdin);
    pembeli->saldoPembeli += topup; FILE *asli=fopen("pembeli.dat","rb"), *sementara=fopen("temp_akun.dat","wb"); struct akun_pembeli akun; bool upd=false;
    if(asli&&sementara){while(fread(&akun,sizeof(struct akun_pembeli),1,asli)==1){if(akun.idPembeli==pembeli->idPembeli){akun.saldoPembeli=pembeli->saldoPembeli;upd=true;}fwrite(&akun,sizeof(struct akun_pembeli),1,sementara);}fclose(asli);fclose(sementara);if(upd){remove("pembeli.dat");rename("temp_akun.dat","pembeli.dat");printf("Top Up Rp%d sukses. Saldo: Rp%d\n",topup,pembeli->saldoPembeli);}else{remove("temp_akun.dat");pembeli->saldoPembeli-=topup;printf("Gagal.\n");}}
    else {pembeli->saldoPembeli-=topup;printf("Error file.\n");} pressEnterToContinue();
}

void lihatRiwayatPembelianOlehPembeli(int idPembeli) {
    FILE* filePesan; struct pesanan psn; bool ada = false;
    filePesan = fopen("pesanan.dat", "rb"); if (filePesan == NULL) { printf("Belum ada riwayat.\n"); pressEnterToContinue(); return; }
    clearScreen(); printf("--- Riwayat Pembelian (ID Pembeli: %d) ---\n", idPembeli);
    printf("%-7s %-10s %-20s %-3s %-10s %s\n", "IDPesn", "IDProd", "Produk", "Jml", "Total", "Status");
    printf("----------------------------------------------------------------------\n");
    while (fread(&psn, sizeof(struct pesanan), 1, filePesan) == 1) if (psn.idPembeli == idPembeli) { ada = true; printf("%-7d %-10d %-20s %-3d Rp%-8d %s\n", psn.idPesanan, psn.idProduk, psn.namaProdukPesanan, psn.jumlahPesanan, psn.totalHargaPesanan, psn.statusPesanan); }
    fclose(filePesan); if (!ada) printf("Kosong.\n");
    pressEnterToContinue();
}

void konfirmasiPesananDiterimaOlehPembeli(int idPembeli) {
    FILE *asli, *sementara; struct pesanan psn; bool upd = false, bisaKonfirm=false; int idPesananKonfirmasi;
    clearScreen(); printf("--- Konfirmasi Pesanan Diterima ---\nPesanan Anda yg bisa dikonfirmasi:\n");
    asli = fopen("pesanan.dat","rb"); if(asli){printf("%-7s %-20s %s\n-------------------------------------------------\n", "IDPesn", "Produk", "Status"); while(fread(&psn,sizeof(struct pesanan),1,asli)==1)if(psn.idPembeli==idPembeli&&(strcmp(psn.statusPesanan,"Dibayar")==0||strcmp(psn.statusPesanan,"Dikirim")==0)){printf("%-7d %-20s %s\n",psn.idPesanan,psn.namaProdukPesanan, psn.statusPesanan);bisaKonfirm=true;}fclose(asli);printf("-------------------------------------------------\n");}
    if(!bisaKonfirm){printf("Tidak ada pesanan u/ dikonfirmasi.\n");pressEnterToContinue();return;}
    printf("ID Pesanan yg diterima: "); if(scanf("%d", &idPesananKonfirmasi)!=1){fflush(stdin);printf("ID Salah.\n");pressEnterToContinue();return;} fflush(stdin);
    asli = fopen("pesanan.dat", "rb"); if (asli == NULL) { printf("File pesanan.dat error.\n"); pressEnterToContinue(); return; }
    sementara = fopen("temp_pesan.dat", "wb"); if (sementara == NULL) { printf("Error file sementara.\n"); fclose(asli); pressEnterToContinue(); return; }
    while (fread(&psn, sizeof(struct pesanan), 1, asli) == 1) { if (psn.idPesanan == idPesananKonfirmasi && psn.idPembeli == idPembeli && (strcmp(psn.statusPesanan,"Dibayar")==0 || strcmp(psn.statusPesanan,"Dikirim")==0) ) { strcpy(psn.statusPesanan, "Diterima"); upd = true; printf("Pesanan ID %d dikonfirmasi.\n", idPesananKonfirmasi); } fwrite(&psn, sizeof(struct pesanan), 1, sementara); }
    fclose(asli); fclose(sementara); if (upd) { remove("pesanan.dat"); rename("temp_pesan.dat", "pesanan.dat"); } else { remove("temp_pesan.dat"); printf("Gagal konfirm.\n"); } pressEnterToContinue();
}

void tulisUlasanOlehPembeli(int idPembeli) {
    FILE* fileUlasan; struct ulasan u; int idProdUlas;
    u.idUlasan = getNextId("ulasanid.dat"); u.idPembeli = idPembeli;
    clearScreen(); printf("--- Tulis Ulasan ---\n"); lihatRiwayatPembelianOlehPembeli(idPembeli);
    printf("\nID Produk yg diulas (0 u/ umum): "); if(scanf("%d", &idProdUlas)!=1){fflush(stdin);printf("ID salah.\n");pressEnterToContinue();return;} fflush(stdin); u.idProduk = idProdUlas;
    printf("Ulasanmu (maks 199 huruf):\n"); fgets(u.isiUlasan, sizeof(u.isiUlasan), stdin); u.isiUlasan[strcspn(u.isiUlasan, "\n")] = 0;
    fileUlasan = fopen("ulasan.dat", "ab"); if (fileUlasan == NULL) { printf("Gagal simpan ulasan.\n"); }
    else { fwrite(&u, sizeof(struct ulasan), 1, fileUlasan); fclose(fileUlasan); printf("Ulasan terkirim!\n"); }
    pressEnterToContinue();
}

