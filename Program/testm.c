#include "./boolean/boolean.h"
#include "./point/point.h"
#include "./point/ListPoint.h"
#include "./arrayKomponen/arrayKomponen.h"
#include "./arrayInventory/arrayInventory.h"
#include "./mesinkarakter/mesinkar.h"
#include "./mesinkarakter/mesinkata.h"
#include "./CircularQueue/circular_queue.h"
#include "./Stack/stack.h"
#include "./matriks/matriks.h"
#include "./graf/graf.h"

#include "./point/point.c"
#include "./point/ListPoint.c"
#include "./arrayKomponen/arrayKomponen.c"
#include "./arrayInventory/arrayInventory.c"
#include "./mesinkarakter/mesinkar.c"
#include "./mesinkarakter/mesinkata.c"
#include "./CircularQueue/circular_queue.c"
#include "./Stack/stack.c"
#include "./matriks/matriks.c"
#include "./graf/graf.c"

#include <stdio.h>
#include <stdlib.h>

// Variabel Global
ArrayKomponen Motherboard,CPU,Memory,CPUCool,Case,GPU,Storage,PSU;
ArrayKomponen All; //gabungan dari seluruh komponen
ListObjek Bangunan; //list menyimpan bangunan dan lokasinya
POINT LokasiPlayer; //lokasi player
ArrayInventory Inventory; //inventory player
MATRIKS Map; //peta game
Graph GrafBangunan; //graf bangunan
Queue Pesanan;
Stack Build;
int uang;
int CurrPelanggan;
int CurrPesanan;

void LOGO(){
    printf("\n");
	printf("                               ____    _    __  _  _______  ____\n");
	printf("                              |  __|  / \\  |  \\| ||__   __||    |\n");
	printf("                              |____  / _ \\ | |\\  |   | |   | || |\n");
	printf("                               |___|/_/ \\_\\|_| \\_|   |_|   |____|\n");
	printf("                              --T.Y.C.O.O.N--  \n");
	printf("\n");
}

void MENU(int *inputmenu){
    printf("                                    Selamat datang di game\n");
    printf("                                         SANTO TYCOON\n");
    printf("                                         1. NEW GAME\n");
    printf("                                         2. LOAD GAME\n");
    printf("                                         >");
    InputAngka(inputmenu);
    printf("\n");
    while (*inputmenu!=1 && *inputmenu!=2){
        printf("                    Input tidak terdefinisi, silahkan masukan angka menu");
        printf("                                     >");
        InputAngka(inputmenu);
    }
}

void KonfigurasiItem(char *path, ArrayKomponen *Komponen, ArrayKomponen *All){
    /* Mengonfigurasi file eksternal komponen ke dalam variabel2 Komponen */
    char Nama[200];
    int Harga;
    char Kategori[20];
    Item Insert;

    *Komponen = MakeArrayKomponen();
    STARTKATA(path);
    ADVKATA();
    while (!EndKata){
        UnionKata(Nama);
        BacaAngka(&Harga);
        UnionKata(Kategori);
        Insert = ArrangeItem(Nama,Harga,Kategori);
        ArrayKomponenInsertLast(Komponen,Insert);
        ArrayKomponenInsertLast(All,Insert);
    }
}

void KonfigurasiMap(char *path,MATRIKS *Map,ListObjek *Objek,POINT *LokPlayer,Graph *GrafB){
    int NBaris;
    int NKolom;
    int NObjek;
    char Simbol;
    int countC = 0;
    char count;
    int X,Y;
    int ElmtM;
    adrNode P;

    STARTKATA(path);
    BacaAngka(&NBaris);
    BacaAngka(&NKolom);
    MakeMATRIKS(NBaris,NKolom,Map);
    InsertKosong(Map);
    BacaAngka(&NObjek);
    MakeListObjek(Objek);

    for (int i = 1; i <= NObjek; i++){
        BacaKarakter(&Simbol);
        BacaAngka(&X);
        BacaAngka(&Y);
        InsertListObjek(Objek,Simbol,X,Y);
        if (Simbol=='C'){
            countC++;
            count = '0' + countC;
            BacaMAP(Map,count,X,Y);
        } else {
            BacaMAP(Map,Simbol,X,Y);
        }
        if (Simbol=='B'){
            *LokPlayer = MakePOINT(X,Y);
        }
    }
    CreateEmptyGraph(GrafB);
    for (int i = 1; i <= NObjek(*Objek); i++)
    {
        InsertNode(GrafB,i,&P);
    }
    for (int i = 1; i <= NObjek(*Objek); i++){
        for (int j = 1; j <= NObjek(*Objek); j++){
            BacaAngka(&ElmtM);
            if (ElmtM==1){
                InsertEdge(GrafB,i,j);
            }
        }
    }
    FINISH();
}

void MOVE(){
    char *NamaLokPlayer;
    int id;
    adrNode NodeLokasiPlayer;
    adrSuccNode DaftarLokasi,NodeLokTujuan;
    int j = 1;
    char *NamaLokasi;
    char *NamaLokasiTujuan;
    int tujuan;

    TulisBangunanLok(Bangunan,LokasiPlayer,&NamaLokPlayer);
    id = SearchIndeks(Bangunan,LokasiPlayer);
    NodeLokasiPlayer=SearchNode(GrafBangunan,id);

    printf("Kamu berada pada %s \n",NamaLokPlayer);
    printf("Daftar lokasi yang dapat dicapai:\n");
    DaftarLokasi = Trail(NodeLokasiPlayer);
    while (DaftarLokasi!=Nil){
        printf("%c\n",Simbol(Bangunan,4));
        TulisBangunanInd(Bangunan,1,&NamaLokasi);
        printf("%d. %s\n",j,NamaLokasi);

        DaftarLokasi = NextG(DaftarLokasi);
        j++;
    }

    printf("Nomor tujuan: ");
    scanf("%d", &tujuan);

    if (1 <= tujuan && tujuan < j){
        int k=1;
        NodeLokTujuan=Trail(NodeLokasiPlayer);
        while (k!=tujuan){
            NodeLokTujuan=NextG(NodeLokTujuan);
            k++;    
        }
        infograph IdTujuan= Id(Succ(NodeLokTujuan));
        POINT LokasiTujuan=Point(Bangunan,IdTujuan);
        
        LokasiPlayer = MakePOINT(Absis(LokasiTujuan),Ordinat(LokasiTujuan));

        TulisBangunanLok(Bangunan,LokasiTujuan,&NamaLokasiTujuan);
        printf("Kamu telah mencapai lokasi %s", NamaLokasiTujuan);
    }
    else{
        printf("Tempat tersebut tidak bisa dituju. Harap pindah ke tempat terdekat telebih dahulu!");
    }
}

void STATUS(){
    printf("Uang tersisa: $%d\n", uang);
    printf("Build yang sedang dikerjakan: pesanan %d untuk Pelanggan %d.\n", &CurrPesanan, &CurrPelanggan);
    printf("Lokasi: pemain sedang berada pada %s.\n", &LokasiPlayer);
    printf("Inventory anda:\n");
    for(int i=0;i<Inventory.Neff;i++){
      printf("%d. %s (%d)\n",(i+1), &(Inventory.A[i].Nama), &(Inventory.A[i].Jumlah));
    }
}

void CHECKORDER(){
    printf("Nomor Order: %s\n", &CurrPesanan);
    printf("Pemesan: Pelanggan %d\n", &CurrPelanggan);
    printf("Invoice: $%d\n", &(Pesanan.Tab[CurrPesanan-1].Nilai));
    printf("Komponen:\n");
    printf("1. %s\n", &(Pesanan.Tab[CurrPesanan-1].Motherboard));
    printf("2. %s\n", &(Pesanan.Tab[CurrPesanan-1].CPU));
    printf("3. %s\n", &(Pesanan.Tab[CurrPesanan-1].Memory));
    printf("4. %s\n", &(Pesanan.Tab[CurrPesanan-1].CPU_Cooler));
    printf("5. %s\n", &(Pesanan.Tab[CurrPesanan-1].Case));
    printf("6. %s\n", &(Pesanan.Tab[CurrPesanan-1].GPU));
    printf("7. %s\n", &(Pesanan.Tab[CurrPesanan-1].Storage));
    printf("8. %s\n", &(Pesanan.Tab[CurrPesanan-1].PSU));   
}

void STARTBUILD(){
    CurrPelanggan=Pesanan.Tab[CurrPesanan].Pemesan;
    CurrPesanan++;
    CreateEmptyStack(&Build);
    printf("Kamu telah memulai pesanan %d untuk Pelanggan %d.\n", CurrPesanan, CurrPelanggan); 
}

void ADDCOMPONENT(){ 
    int x;
    printf("Komponen yang telah terpasang:\n");
    for(int i=0;i<TOP(Build);i++){
        printf("%d. %s\n", (i+1), Build.T[i]);
    }
    printf("Komponen yang tersedia\n");
    for(int i=0;i<Inventory.Neff;i++){
        printf("&d. %s\n", (i+1), (Inventory.A[i].Nama));
    }

    printf("Komponen yang ingin dipasang: \n");
    scanf("%d", &x);
    if (x <= Inventory.Neff){ /*masih gatau gimana caranya ngilangin komponen yang tersedia kalo dipilih buat dipasang */
        Push(&Build, Inventory.A[x-1].Nama);
        Inventory.A[x-1].Jumlah--;
        if (Inventory.A[x-1].Jumlah==0){
            for(int i=x-1;i<(Inventory.Neff-1);i++){
                Inventory.A[x-1]=Inventory.A[x];
            }
            Inventory.Neff--;
        }
        printf("Komponen berhasil dipasang! \n");
    }
    else{
        printf("Komponen tidak tersedia!");
    }   
}

void REMOVECOMPONENT(){
    char *copot;
    Pop(&Build,&copot);
    Element simpan;
    simpan.Nama=copot;
    simpan.Jumlah=1;
    ArrayInventoryInsertLast(&Inventory,simpan);
    printf("Komponen %d\n", &copot, " berhasil dicopot!");
}

void FINISHBUILD(){
    boolean benar=true;
    if(Build.T[1]!=Pesanan.Tab[CurrPesanan-1].Motherboard){
        benar=false;
    }
    if(Build.T[2]!=Pesanan.Tab[CurrPesanan-1].CPU){
        benar=false;
    }
    if(Build.T[3]!=Pesanan.Tab[CurrPesanan-1].Memory){
        benar=false;
    }
    if(Build.T[4]!=Pesanan.Tab[CurrPesanan-1].CPU_Cooler){
        benar=false;
    }
    if(Build.T[5]!=Pesanan.Tab[CurrPesanan-1].Case){
        benar=false;
    }
    if(Build.T[6]!=Pesanan.Tab[CurrPesanan-1].GPU){
        benar=false;
    }
    if(Build.T[7]!=Pesanan.Tab[CurrPesanan-1].Storage){
        benar=false;
    }
    if(Build.T[8]!=Pesanan.Tab[CurrPesanan-1].PSU){
        benar=false;
    }
      
    if(benar){ 
        printf("%d", &CurrPesanan, "telah selesai. Silahkan antar ke Pelanggan %d\n", &CurrPelanggan);
        Element Komputer;
        Komputer.Nama="Build untuk pesanan #%d",&CurrPesanan;
        Komputer.Jumlah=1;
        ArrayInventoryInsertLast(&Inventory,Komputer);
    }
    else{
        printf("Komponen yang dipasangkan belum sesuai dengan pesanan, build belum dapat diselesaikan");
    }
}

void DELIVER(){
    if (EQPOINT(LokasiPlayer,Bangunan.TabObjek[CurrPelanggan+2].Loc)){
        printf("Pesanan #%d berhasil diantarkan kepada Pelanggan %d!\n", &CurrPesanan, &CurrPelanggan);    
    }
    else{
        printf("Posisi anda belum berada di rumah Pelanggan %d. Harap pindah ke rumah Pelanggan %d terlebih dahulu!\n", &CurrPelanggan, &CurrPelanggan);
    }
}

void SHOP(){
    int pil, pilkom, jumlah, pilawal;
	ArrayKomponen X;

    printf("1. Tampilkan semua komponen");
	printf("2. Tampilkan komponen berdasarkan tipe");
	printf("Masukkan pilihan: ");
	scanf("%d", &pilawal);
	if (pilawal == 1)
	{
		for (int i=0;i<All.Neff;i++)
		{
			printf("%d. %s $%d\n" , i+1,All.A[i].Nama,All.A[i].Harga);
		}
		printf("Komponen yang ingin dibeli: ");
		scanf("%d", &pilkom);
		printf("\n");
		printf("Masukkan jumlah yang ingin dibeli: ");
		scanf("%d", &jumlah);
		printf("\n");

		if ((All.A[pilkom-1].Harga * jumlah) >= uang)
		{
			printf("Kompenen berhasil dibeli!");
			uang = uang - All.A[pilkom-1].Harga * jumlah;
			Element Bought;
            Bought.Nama = All.A[pilkom-1].Nama;
            Bought.Jumlah = jumlah;
            ArrayInventoryInsertLast(&Inventory, Bought);
		}
		else
		{
			printf("Uang tidak cukup!");
		}
	}
	else
	{
		printf("Tipe komponen:\n");
	    printf("1. Motherboard\n");
		printf("2. CPU\n");
		printf("3. Memory\n");
		printf("4. CPU Cooler\n");
		printf("5. Case\n");
		printf("6. GPU\n");
		printf("7. Storage\n");
		printf("8. PSU\n");
		printf("Masukkan pilihan: ");
		scanf("%d", &pil);
		if (pil == 1)
		{
			X.A = Motherboard.A;
			X.Capacity = Motherboard.Capacity;
			X.Neff = Motherboard.Neff;
		}
		else if (pil == 2)
		{
			X.A = CPU.A;
			X.Capacity = CPU.Capacity;
			X.Neff = CPU.Neff;
		}
	    else if (pil == 3)
		{
			X.A = Memory.A;
			X.Capacity = Memory.Capacity;
			X.Neff = Memory.Neff;
		}
		else if (pil == 4)
		{
			X.A = CPUCool.A;
			X.Capacity = CPUCool.Capacity;
			X.Neff = CPUCool.Neff;
		}
		else if (pil == 5)
		{
			X.A = Case.A;
			X.Capacity = Case.Capacity;
			X.Neff = Case.Neff;
		}
		else if (pil == 6)
		{
			X.A = GPU.A;
			X.Capacity = GPU.Capacity;
			X.Neff = GPU.Neff;
		}
		else if (pil == 7)
		{
			X.A = Storage.A;
	    	X.Capacity = Storage.Capacity;
			X.Neff = Storage.Neff;
		}
		else if (pil == 8)
		{
			X.A = PSU.A;
			X.Capacity = PSU.Capacity;
			X.Neff = PSU.Neff;
		}
		for (int i=0;i<X.Neff;i++)
		{
			printf("%d. %s $%d\n" , i+1,X.A[i].Nama,X.A[i].Harga);
		}
		printf("Komponen yang ingin dibeli: ");
		scanf("%d", &pilkom);
		printf("\n");
		printf("Masukkan jumlah yang ingin dibeli: ");
		scanf("%d", &jumlah);
		printf("\n");

		if ((X.A[pilkom-1].Harga * jumlah) >= uang)
		{
			printf("Kompenen berhasil dibeli!");
			uang = uang - X.A[pilkom-1].Harga * jumlah;
			Element Bought;
            Bought.Nama = X.A[pilkom-1].Nama;
            Bought.Jumlah = jumlah;
            ArrayInventoryInsertLast(&Inventory, Bought);
		}
		else
		{
			printf("Uang tidak cukup!");
		}
	}
}

int Random(int lower, int upper){
    srand(time(NULL));

    return(rand() % (upper-lower+1)+lower);    
}

void END_DAY(){
    int pemesan;
    int K1, K2, K3, K4, K5, K6, K7, K8, sumK, tagihan;
    int K1H, K2H, K3H, K4H, K5H, K6H, K7H, K8H;
    Order PSN; 
    pemesan = Random(1,7);
    PSN.Pemesan = pemesan;
        
    K1 = (Random(0,35)%5);
    K1H = Motherboard.A[K1].Harga;
    PSN.Motherboard = Motherboard.A[K1].Nama;

    K2 = (Random(21,65)%5);
    K2H = CPU.A[K2].Harga;
    PSN.CPU = CPU.A[K2].Nama;

    K3 = (Random(85,150)%5);
    K3H = Memory.A[K3].Harga;
    PSN.Memory = Memory.A[K3].Nama;

    K4 = (Random(78, 98)%5);
    K4H = CPUCool.A[K4].Harga;
    PSN.CPU_Cooler = CPUCool.A[K4].Nama;

    K5 = (Random(101, 111)%5);
    K5H = Case.A[K5].Harga;
    PSN.Case = Case.A[K5].Nama;

    K6 = (Random(212, 232)%5);
    K6H = GPU.A[K6].Harga;
    PSN.GPU = GPU.A[K6].Nama;

    K7 = (Random(9, 79)%5);
    K7H = Storage.A[K7].Harga;
    PSN.Storage = Storage.A[K7].Nama;

    K8 = (Random(20, 86)%5);
    K8H = PSU.A[K8].Harga;
    PSN.PSU = PSU.A[K8].Nama;

    sumK = K1H+K2H+K3H+K4H+K5H+K6H+K7H+K8H;
    tagihan = sumK * 1.5;
    PSN.Nilai = tagihan;

    PushQueue(&Pesanan, PSN);
}

void MAP(){
    TulisMAP(Map, Absis(LokasiPlayer), Ordinat(LokasiPlayer));
}

void COMMAND()
/* Menginput COMMAND, mengecek commandnya, serta menjalankan commandnya */
{
    boolean exit = false;
    while (!exit){
        printf("ENTER COMMAND: ");
        InputUSER();
        if (IsKataMOVE(CKata)){
            MOVE();
        } else if (IsKataSTATUS(CKata)){
            STATUS();
        } else if (IsKataCHECKORDER(CKata)){
            CHECKORDER();
        } else if (IsKataSTARTBUILD(CKata)){
            STARTBUILD();
        } else if (IsKataFINISHBUILD(CKata)){
            FINISHBUILD();
        } else if (IsKataADDCOMPONENT(CKata)){
            ADDCOMPONENT();
        } else if (IsKataREMOVECOMPONENT(CKata)){
            REMOVECOMPONENT();
        } else if (IsKataSHOP(CKata)){
            SHOP();
        } else if (IsKataDELIVER(CKata)){
            DELIVER();
        } else if (IsKataEND_DAY(CKata)){
            END_DAY();
        } else if (IsKataMAP(CKata)){
            MAP();
        } else if (IsKataEXIT(CKata)){
            exit = true;
        } else if (IsKataSAVE(CKata)){

        } else {
            printf("COMMAND tidak terdefinisi. Silahkan input COMMAND lain!\n");
        }
    }
}

int main(){
    /* Variabel */
    int inputmenu;
    All = MakeArrayKomponen();

    /* Program */
    LOGO();
    MENU(&inputmenu);
    if (inputmenu==1){
        //Konfigurasi File Eksternal (Komponen/Item, Map, Objek)
        KonfigurasiItem("./File eksternal/Motherboard.txt", &Motherboard, &All);
        KonfigurasiItem("./File eksternal/CPU.txt", &CPU, &All);
        KonfigurasiItem("./File eksternal/Memory.txt", &Memory, &All);
        KonfigurasiItem("./File eksternal/CPU Cooler.txt", &CPUCool, &All);
        KonfigurasiItem("./File eksternal/Case.txt", &Case, &All);
        KonfigurasiItem("./File eksternal/GPU.txt", &GPU, &All);
        KonfigurasiItem("./File eksternal/Storage.txt", &Storage, &All);
        KonfigurasiItem("./File eksternal/PSU.txt", &PSU, &All);
        KonfigurasiMap("./File eksternal/Map.txt", &Map, &Bangunan, &LokasiPlayer, &GrafBangunan);
        printf("%d\n",isConnected(GrafBangunan,1,2));
        //Inventory Awal
        Inventory = MakeArrayInventory();

        //Order Pesawan Awal
        uang = 10000; // inisialisasi uang
        Pesanan = CreateQueue(99);
        END_DAY();
        COMMAND();
    } else {

    }
    
}
