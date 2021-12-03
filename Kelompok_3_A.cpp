#include <iostream>
#include <conio.h>
#include <bits/stdc++.h>
#include <sstream>
#include <string>
using namespace std;

struct DaftarMenu{
    string nama;
    float harga;
    int stok;
};

struct Pemesanan{
    string namamenu;
    int banyak;
    int totalharga;
};

struct Akun{
    string username, password, nama, no_hp, alamat;
};

struct NodeOrderan{
    int banyakmenu, totalbiaya;
    struct Akun pemesan;
    struct Pemesanan pesanan[10];
    struct NodeOrderan *next;
};

struct NodeAkun{
    struct Akun account;
    struct NodeAkun *next;
};

struct Node{
    struct DaftarMenu menu;
    struct Node *next;
    struct Node *prev;
};

struct DaftarMenu arr[50], temp;
struct Akun acc[50];
struct Pemesanan psn[50];
struct Node *head = NULL, *tail = NULL, *del;
struct NodeAkun *headacc = NULL;
struct NodeOrderan *headord = NULL, *del2, ord[50];
int lastNodeIndex, lastNodeAccount, lastNodeOrder, countArrayData, countArrayAccount, countArrayOrder;
int menuChanged, akunChanged, indexAkun, indexBelanja, quantityBelanja, sorted;
void customer();

void lihatMenu(Node *head, int role){
    if (head == NULL){
        cout << "Menu belum terbaca." << endl;
        return;
    }
    int i; lastNodeIndex = 0;
    if (role == 1){
        cout << "[No] [Stok] [Harga]\t[Menu]" << endl;
        while (head != NULL){
            if (i < 9) cout << "[0"<<i+1<<"] " << head->menu.stok 
            << "     Rp" << head->menu.harga 
            << "\t" << head->menu.nama << endl;
            else cout << "["<<i+1<<"] " << head->menu.stok 
            << "     Rp" << head->menu.harga 
            << "\t" << head->menu.nama << endl;
            i++; lastNodeIndex++;
            head = head->next;
        }
    } else if (role == 2){
        cout << "[No] [Harga]\t[Menu]" << endl;
        while (head != NULL){
            if (i < 9) cout << "[0"<<i+1<<"] Rp"<<head->menu.harga<<"\t"<<head->menu.nama<<endl;
            else cout << "["<<i+1<<"] Rp"<<head->menu.harga<<"\t"<<head->menu.nama<<endl;
            i++; lastNodeIndex++;
            head = head->next;
        }
    }
}

void editMenu(Node *node){
    int nomor;
    cout << "Edit No: "; cin >> nomor;
    if (lastNodeIndex == 0){
        cout << "ERROR: Data masih kosong." << endl;
        system("pause");
        return;
    }
    if (nomor > lastNodeIndex || nomor < 1){
        cout << "ERROR: Offside, bro. Ga ada data disitu." << endl;
        system("pause");
        return;
    }
    for (int i = 1; i < nomor; i++) node = node->next; // agar sampai ke data yang diinginkan
    if (node == NULL) { // jika node kosong
        cout << "Data tidak ditemukan." << endl;
        system("pause");
        return;
    }
    cout << "\nMenu ke-" << nomor << ":" << endl;
    cout << "Nama  : "; cin.ignore(); getline(cin, node->menu.nama);
    cout << "Harga : "; cin >> node->menu.harga;
    cout << "Stok  : "; cin >> node->menu.stok;
    cout << "Data telah diupdate." << endl;
    menuChanged = 1;
    system("pause");
}

void tambahMenu(Node **head, Node **tail){
    int n, posisi;
    cout << "Tambah Menu sebanyak: "; cin >> n;
    int countPosisi = posisi + n-1;
    for (int i = 0; i < n; i++){
        Node *newNode = new Node();
        cout << "\nMenu ke-" << lastNodeIndex+1 << ":" << endl;
        cout << "Nama  : "; cin.ignore(); getline(cin, newNode->menu.nama);
        cout << "Harga : "; cin >> newNode->menu.harga;
        cout << "Stok  : "; cin >> newNode->menu.stok;
        countPosisi--;
        if (*head == NULL){
            *head = newNode;
            *tail = newNode;
        } else {
            newNode->next = NULL;
            Node *current = *head;
            while (current->next != NULL){
                current = current->next;
            }
            current->next = newNode;
            newNode->prev = current;
        }
        lastNodeIndex++; posisi++;
    }
    menuChanged = 1;
}

void hapusMenu(Node **head, Node **tail){
    if (*head == NULL){
        cout << "Menu Kosong" << endl;
        system("pause");
        lastNodeIndex = 0;
        return;
    }
    Node *current = *head;
    int position;
    cout << "Delete data at no: "; cin >> position;
    if (position == 0) return;
    if (position < 1 || position > lastNodeIndex){
        cout << "Data tidak ditemukan." << endl;
        system("pause");
        return;
    }
    // Traversal dari awal ke posisi yang diinginkan
    for (int i = 1; current != NULL && i < position; i++) current = current->next;
    // Jika node yang ingin dihapus == Kosong, maka
    if (current == NULL) return;
    // Jika node yang ingin dihapus adalah node awal
    if (*head == current) *head = current->next;
    // Jika nodenya terletak pertama, masukkan PREV pada Current ke PREV pada Node berikutnya
    if (current->next != NULL) current->next->prev = current->prev;
    // Jika nodenya terletak terakhir, masukkan NEXT pada Current ke NEXT pada Node sebelumnya 
    if (current->prev != NULL) current->prev->next = current->next;
    free(current);
    lastNodeIndex--;
    menuChanged = 1; 
}

void SortedArrayKeNodeMenu(Node *node){
    for (int i = 0; i < countArrayData && node != NULL; i++){
        node->menu.nama = arr[i].nama;
        node->menu.harga = arr[i].harga;
        node->menu.stok = arr[i].stok;
        node = node->next;
    }
}

void NodeKeArrayMenu(DaftarMenu *arr, Node *node){
    countArrayData = 0;
    for (int i = 0; i < lastNodeIndex && node != NULL; i++){
        arr[i].nama = node->menu.nama;
        arr[i].harga = node->menu.harga;
        arr[i].stok = node->menu.stok;
        node = node->next;
        countArrayData++;
    }
}

void ArrayKeNodeMenu(DaftarMenu *arr, Node **head, Node **tail){
    lastNodeIndex = 0;
    for (int i = 0; i < countArrayData; i++){
        Node *newNode = new Node();
        newNode->menu.nama = arr[i].nama;
        newNode->menu.harga = arr[i].harga;
        newNode->menu.stok = arr[i].stok;
        if (*head == NULL){
            *head = newNode;
            *tail = newNode;
        } else {
            newNode->next = NULL;
            Node *current = *head;
            while (current->next != NULL){
                current = current->next;
            }
            current->next = newNode;
            newNode->prev = current;
        }
        lastNodeIndex++;
    }
}

void NodeKeArrayAkun(Akun *acc, NodeAkun *nodeacc){
    countArrayAccount = 0;
    for (int i = 0; i < lastNodeAccount && nodeacc != NULL; i++){
        acc[i].username = nodeacc->account.username;
        acc[i].password = nodeacc->account.password;;
        acc[i].nama = nodeacc->account.nama;
        acc[i].alamat = nodeacc->account.alamat;
        acc[i].no_hp = nodeacc->account.no_hp;
        nodeacc = nodeacc->next;
        countArrayAccount++;
    }
}

void ArrayKeNodeAkun(Akun *acc, NodeAkun **headacc){
    lastNodeAccount = 0;
    for (int i = 0; i < countArrayAccount; i++){
        NodeAkun *newNode = new NodeAkun();
        newNode->account.username = acc[i].username;
        newNode->account.password = acc[i].password;
        newNode->account.nama = acc[i].nama;
        newNode->account.alamat = acc[i].alamat;
        newNode->account.no_hp = acc[i].no_hp;
        if (*headacc == NULL){
            *headacc = newNode;
        } else {
            newNode->next = NULL;
            NodeAkun *current = *headacc;
            while (current->next != NULL){
                current = current->next;
            }
            current->next = newNode;
        }
        lastNodeAccount++;
    }
}

void tambahAkun(NodeAkun **headacc){
    cout << "===========================================" << endl;
    cout << "                TAMBAH AKUN                " << endl;
    cout << "===========================================" << endl;
    NodeAkun *newNode = new NodeAkun();
    cout << "\nAkun ke-" << lastNodeAccount+1 << ":" << endl;
    cout << "Username: "; cin >> newNode->account.username;
    cout << "Password: "; cin >> newNode->account.password;
    cout << "Nama    : "; cin.ignore(); getline(cin, newNode->account.nama);
    cout << "No.HP   : "; cin >> newNode->account.no_hp;
    cout << "Alamat  : "; cin >> newNode->account.alamat;
    if (*headacc == NULL){
        *headacc = newNode;
    } else {
        newNode->next = NULL;
        NodeAkun *current = *headacc;
        while (current->next != NULL) current = current->next;
        current->next = newNode;
    }
    akunChanged = 1; 
}

void editAkun(NodeAkun *nodeacc){
    cout << "===========================================" << endl;
    cout << "                 EDIT AKUN                 " << endl;
    cout << "===========================================" << endl;
    int nomor;
    cout << "Edit No: "; cin >> nomor;
    if (lastNodeAccount == 0 || nomor > lastNodeAccount || nomor < 1){
        cout << "ERROR: Data tidak ditemukan." << endl;
        system("pause");
        return;
    }
    for (int i = 1; i < nomor; i++) nodeacc = nodeacc->next;
    if (nodeacc == NULL) { 
        cout << "Error: Data tidak ditemukan." << endl;
        system("pause");
        return;
    }
    cout << "\nAkun ke-" << nomor << ":" << endl;
    cout << "Username: "; cin >> nodeacc->account.username;
    cout << "Password: "; cin >> nodeacc->account.password;
    cout << "Nama    : "; cin.ignore(); getline(cin, nodeacc->account.nama);
    cout << "No.HP   : "; cin >> nodeacc->account.no_hp;
    cout << "Alamat  : "; cin >> nodeacc->account.alamat;
    cout << "Data telah diupdate." << endl;
    akunChanged = 1;
    system("pause");
}

void hapusAkun(NodeAkun **headacc){
    cout << "===========================================" << endl;
    cout << "                 HAPUS AKUN                " << endl;
    cout << "===========================================" << endl;
    if (*headacc == NULL){
        cout << "Menu Kosong" << endl;
        system("pause");
        lastNodeAccount = 0;
        return;
    }
    int nomor;
    cout << "Hapus Akun No: "; cin >> nomor;
    if (nomor == 0) return;
    if (nomor < 1 || nomor > lastNodeAccount){
        cout << "Data tidak ditemukan." << endl;
        system("pause");
        return;
    }
    if((*headacc)->next == NULL){
        *headacc = NULL;
        akunChanged = 1; 
        return;
    }
    NodeAkun *current = *headacc;
    while(current->next->next != NULL) current = current->next;
    current->next = NULL;
    free(current);
    lastNodeAccount--;
    akunChanged = 1; 
}

void NodeKeArrayOrder(NodeOrderan *ord, NodeOrderan *nodeord){
    countArrayOrder = 0;
    for (int i = 0; i < lastNodeOrder && nodeord != NULL; i++){
        ord[i].pemesan.nama = nodeord->pemesan.nama;
        ord[i].pemesan.no_hp = nodeord->pemesan.no_hp;
        ord[i].pemesan.alamat = nodeord->pemesan.alamat;
        ord[i].banyakmenu = nodeord->banyakmenu;
        ord[i].totalbiaya = nodeord->totalbiaya;
        for (int j = 0; j < nodeord->banyakmenu; j++){
            ord[i].pesanan[j].namamenu = nodeord->pesanan[j].namamenu;
            ord[i].pesanan[j].banyak = nodeord->pesanan[j].banyak;
            ord[i].pesanan[j].totalharga = nodeord->pesanan[j].totalharga;
        }
        nodeord = nodeord->next;
        countArrayOrder++;
    }
}

void ArrayKeNodeOrder(NodeOrderan *ord, NodeOrderan **headord){
    lastNodeOrder = 0;
    for (int i = 0; i < countArrayOrder; i++){
        NodeOrderan *newNode = new NodeOrderan();
        newNode->banyakmenu = ord[i].banyakmenu;
        newNode->totalbiaya = ord[i].totalbiaya;
        newNode->pemesan.nama = ord[i].pemesan.nama;
        newNode->pemesan.alamat = ord[i].pemesan.alamat;
        newNode->pemesan.no_hp = ord[i].pemesan.no_hp;
        for (int j = 0; j < ord[i].banyakmenu; j++){
            newNode->pesanan[j].namamenu = ord[i].pesanan[j].namamenu;
            newNode->pesanan[j].banyak = ord[i].pesanan[j].banyak;
            newNode->pesanan[j].totalharga = ord[i].pesanan[j].totalharga;
        }
        if (*headord == NULL){
            *headord = newNode;
        } else {
            newNode->next = NULL;
            NodeOrderan *current = *headord;
            while (current->next != NULL){
                current = current->next;
            }
            current->next = newNode;
        }
        lastNodeOrder++;
    }
}

void urutMenu(DaftarMenu *arr, int size){
    for (int gap = size/2; gap > 0; gap /= 2) {
        for (int i = 0; i < size; i += 1){
            temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j-gap].nama > temp.nama; j -= gap) arr[j] = arr[j-gap];
            arr[j] = temp;
        }
    }
    sorted = 1;
    SortedArrayKeNodeMenu(head);
    menuChanged = 1;
}

int searchMenu(DaftarMenu *arr, string x, int n){
	int step = sqrt(n);
	int prev = 0;
	while (arr[min(step, n) - 1].nama < x){
		prev = step;
		step += sqrt(n);
		if (prev >= n) return -1;
	}
	while (arr[prev].nama < x){
		prev++; if (prev == min(step, n)) return -1;
	}
	if (arr[prev].nama == x) return prev;
	return -1;
}

void cariMenu(){
    string findText;
    if (sorted != 1) urutMenu(&arr[0], lastNodeIndex);
    cout << "Cari Menu > "; cin.ignore(); getline(cin, findText);
    int i = searchMenu(&arr[0], findText, lastNodeIndex);
    if (i != -1){
        cout << "Data ditemukan!" << endl;
        cout << "Nama  : " << arr[i].nama<< endl;
        cout << "Harga : " << arr[i].harga << endl;
        cout << "Stok  : " << arr[i].stok << endl;
    } else {
        cout << "Data tidak ditemukan!" << endl;
    }
    system("pause");
}

void orderanSelesai(NodeOrderan **headord){
    if (*headord == NULL){
        cout << "Orderan Kosong" << endl;
        system("pause");
        lastNodeIndex = 0;
        return;
    }
    del2 = *headord;
    *headord = (*headord)->next;
    delete del2;
}

void orderanMasuk(NodeOrderan **headord, Pemesanan *psn, Akun *acc, int totalharga){
    NodeOrderan *newNode = new NodeOrderan();
    newNode->pemesan.nama = acc[indexAkun].nama;
    newNode->pemesan.no_hp = acc[indexAkun].no_hp;
    newNode->pemesan.alamat = acc[indexAkun].alamat;
    newNode->banyakmenu = quantityBelanja;
    newNode->totalbiaya = totalharga;
    for (int i = 0; i < quantityBelanja; i++){
        newNode->pesanan[i].namamenu = psn[i].namamenu;
        newNode->pesanan[i].banyak = psn[i].banyak;
        newNode->pesanan[i].totalharga = psn[i].totalharga;
    }
    if (*headord == NULL){
        *headord = newNode;
    } else {
        newNode->next = NULL;
        NodeOrderan *current = *headord;
        while (current->next != NULL){
            current = current->next;
        }
        current->next = newNode;
    }
    lastNodeOrder++;
}

void ArraykeCSV(DaftarMenu *arr, Akun *acc, NodeOrderan *ord, int type){
    fstream fout;
    if (type == 1){
        fout.open("menurestoran.csv", ios::out);
        for (int i = 0; i < countArrayData; i++) {
            fout << arr[i].nama << "," 
            << arr[i].harga << "," 
            << arr[i].stok << "\n";
        }
        fout.close();
    } else if (type == 2){
        fout.open("akun.csv", ios::out);
        for (int i = 0; i < countArrayAccount; i++) {
            fout << acc[i].username << "," 
            << acc[i].password << "," 
            << acc[i].nama << "," 
            << acc[i].alamat << "," 
            << acc[i].no_hp << "\n";
        }
        fout.close();
    } else if (type == 3){
        fout.open("orderan.csv", ios::out);
        for (int i = 0; i < countArrayOrder; i++) {
            fout << ord[i].pemesan.nama << ","
            << ord[i].pemesan.no_hp << ","
            << ord[i].pemesan.alamat << ","
            << ord[i].banyakmenu << ","
            << ord[i].totalbiaya << ",";
            for (int j = 0; j < ord[i].banyakmenu; j++){
                fout << ord[i].pesanan[j].namamenu << ","
                << ord[i].pesanan[j].banyak << "," 
                << ord[i].pesanan[j].totalharga;
                if (ord[i].banyakmenu - j == 1) fout << "\n";
                else fout << ",";
            }
        }
        fout.close();
    }
}

void CSVkeArray(DaftarMenu *arr, Akun *acc, NodeOrderan *ord){
    fstream fin, fin2, fin3;
    fin.open("menurestoran.csv", ios::in);
    fin2.open("akun.csv", ios::in);
    fin3.open("orderan.csv", ios::in);
    string nama_s, harga_s, stok_s, line, line2, line3;
    string user, pass, nama, namamenu, almt, nohp, bnykmenu_s, totalbiaya_s, banyak_s, totalharga_s; 
    int stok_i, banyakmenu, totalbiaya, banyak, totalharga, harga_i;
    if (fin.is_open()){
        while(getline(fin, line)){
            stringstream ss(line);
            getline(ss, nama_s, ',');
			getline(ss, harga_s, ',');
			getline(ss, stok_s, '\n');
            stringstream ss2(harga_s);
            stringstream ss3(stok_s);
            ss2 >> harga_i;
            ss3 >> stok_i;
            arr[countArrayData].nama = nama_s;
            arr[countArrayData].harga = harga_i;
            arr[countArrayData].stok = stok_i;
            countArrayData++;
        }
        fin.close();
    } if (fin2.is_open()){
        while(getline(fin2, line2)){
            stringstream ss4(line2);
            getline(ss4, user, ',');
			getline(ss4, pass, ',');
			getline(ss4, nama, ',');
			getline(ss4, almt, ',');
			getline(ss4, nohp, '\n');
            acc[countArrayAccount].username = user;
            acc[countArrayAccount].password = pass;
            acc[countArrayAccount].nama = nama;
            acc[countArrayAccount].alamat = almt;
            acc[countArrayAccount].no_hp = nohp;
            countArrayAccount++;
        }
    } if (fin3.is_open()) {
        while(getline(fin3, line3)){
            stringstream ss5(line3);
            getline(ss5, nama, ',');
			getline(ss5, nohp, ',');
			getline(ss5, almt, ',');
			getline(ss5, bnykmenu_s, ',');
			getline(ss5, totalbiaya_s, ',');
            stringstream ss6(bnykmenu_s);
            stringstream ss7(totalbiaya_s);
            ss6 >> banyakmenu;
            ss7 >> totalbiaya;
            for (int i = 0; i < banyakmenu; i++){
                getline(ss5, namamenu, ',');
                getline(ss5, banyak_s, ',');
                stringstream ss8(banyak_s);
                ss8 >> banyak;
                if (banyakmenu - i == 1) getline(ss5, totalharga_s, '\n');
                else getline(ss5, totalharga_s, ',');
                stringstream ss9(totalharga_s);
                ss9 >> totalharga;
                ord[countArrayOrder].pesanan[i].namamenu = namamenu;
                ord[countArrayOrder].pesanan[i].banyak = banyak;
                ord[countArrayOrder].pesanan[i].totalharga = totalharga;
            }
            ord[countArrayOrder].pemesan.nama = nama;
            ord[countArrayOrder].pemesan.alamat = almt;
            ord[countArrayOrder].pemesan.no_hp = nohp;
            ord[countArrayOrder].banyakmenu = banyakmenu;
            ord[countArrayOrder].totalbiaya = totalbiaya;
            countArrayOrder++;
        }
    } else {
        cout << "ERROR: CSV tidak ditemukan." << endl;
        system("pause");
        return;
    }
}

void strukBelanja(Pemesanan *psn, Akun *acc){
    int totalhargapesanan = 15000, confirm, confirm2;
    system("cls");
    cout << "=========================" << endl;
    cout << "Nama    : " << acc[indexAkun].nama << endl;
    cout << "No.HP   : " << acc[indexAkun].no_hp << endl;
    cout << "Alamat  : " << acc[indexAkun].alamat << endl;
    cout << "Pesanan : " << endl;
    for (int i = 0; i < quantityBelanja; i++) {
        cout << psn[i].banyak << " x " << psn[i].namamenu << " = " << psn[i].totalharga << endl;
        totalhargapesanan += psn[i].totalharga;
    }
    cout << "\nTotal + Ongkir (Rp15000): " << endl << totalhargapesanan << endl;
    while (true){
        cout << "=========================" << endl
        << "[1] Bayar Pesanan" << endl
        << "[0] Batalkan Pesanan" << endl
        << "[>] "; cin >> confirm;
        if (confirm == 1){
            orderanMasuk(&headord, &psn[0], &acc[0], totalhargapesanan);
            cout<< "=========================" << endl
                << "GOPAY, OVO, DANA:" << endl
                << "081250342450" << endl
                << "Pesanan akan diproses" << endl 
                << "setelah pesanan dibayar." << endl
                << "Terima Kasih~" << endl
                << "=========================" << endl;
                system("pause");
            break;
        } else if (confirm == 0){
            for (int i = 0; i < quantityBelanja; i++) {
                psn[i].namamenu = "";
                psn[i].banyak = 0;
                psn[i].totalharga = 0;
            }
            break;
        } else {
            cout << "Error: Pilihan tidak tersedia." << endl;
            system("pause");
        }
    }
    quantityBelanja = 0;
}

void pesanMenu(Node *node, Pemesanan *psn, int selectBuy) {
    int pesanSebanyak = -1; char confirm;
    for (int i = 1; i < selectBuy; i++) node = node->next;
    cout << "Pilihan Menu : " << node->menu.nama << endl;
    cout << "Harga Menu   : " << node->menu.harga << endl;
    cout << "Tekan 0 untuk batal memesan." << endl;
    while (true){
        cout << "Pesan Sebanyak > "; cin >> pesanSebanyak;
        if (pesanSebanyak > node->menu.stok) {
            cout << "Maaf! Stok menu ini hanya tersisa " << node->menu.stok << " porsi." << endl;
            system("pause");
        } else if (pesanSebanyak < 0) {
            cout << "Tidak bisa memesan kurang dari 0." << endl;
            system("pause");
        } else if (pesanSebanyak == 0){
            break;
        } else {
            node->menu.stok -= pesanSebanyak;
            psn[indexBelanja].namamenu = node->menu.nama;
            psn[indexBelanja].banyak = pesanSebanyak;
            psn[indexBelanja].totalharga = pesanSebanyak * node->menu.harga;
            cout << psn[indexBelanja].banyak << " * " 
                << psn[indexBelanja].namamenu << " = " 
                << psn[indexBelanja].totalharga << endl;
            quantityBelanja++;
            while (confirm != 'n'){
                cout << "Pesan menu lain? (y/n): "; cin >> confirm;
                if (confirm == 'y' || confirm == 'Y'){
                    indexBelanja++;
                    break;
                } else if (confirm == 'n' || confirm == 'N'){
                    return strukBelanja(&psn[0], &acc[0]); 
                } else {
                    cout << "Error: Tekan 'y' atau 'n'." << endl;
                }
            }
            break;
        }
    }
}

void lihatUser(NodeAkun *headacc){
    if (headacc == NULL){
        cout << "Akun belum terbaca." << endl;
        system("pause");
        return;
    }
    int i = 0; lastNodeAccount = 0;
    system("cls");
    while (headacc != NULL){
        if (i < 9) 
        cout << "[0" << i+1 << "]" << endl <<
        "Username : " << headacc->account.username << endl <<
        "Nama     : " << headacc->account.nama <<  endl <<
        "Alamat   : " << headacc->account.alamat <<  endl <<
        "No. HP   : " << headacc->account.no_hp << endl;
        else cout << "[" << i+1 << "]" << endl <<
        "Username : " << headacc->account.username <<
        "Nama     : " << headacc->account.nama << 
        "Alamat   : " << headacc->account.alamat <<
        "No. HP   : " << headacc->account.no_hp << endl;
        i++; lastNodeAccount++;
        headacc = headacc->next;
    }
    system("pause");
}

void lihatOrderan(NodeOrderan *headord){
    if (headord == NULL){
        cout << "Orderan Kosong." << endl;
        return;
    }
    int no; lastNodeOrder = 0;
    while (headord != NULL){
        if (no < 9) cout << "[0"<<no+1<<"] " << endl;
        else cout << "["<<no+1<<"] " << endl;
        cout <<
        "Nama    : " << headord->pemesan.nama << endl <<
        "No HP   : " << headord->pemesan.no_hp << endl <<
        "Alamat  : " << headord->pemesan.alamat << endl <<
        "Pesanan : " << endl;
        for (int i = 0; i < headord->banyakmenu; i++) {
            cout << headord->pesanan[i].banyak << " * " 
            << headord->pesanan[i].namamenu << " = " 
            << headord->pesanan[i].totalharga << endl;
        }
        cout << "Total   : " << headord->totalbiaya << endl << endl;
        no++; lastNodeOrder++;
        headord = headord->next;
    }
}

void admin(){  
    int selectUtama, selectAdmin1, selectAdmin2, selectAdmin3;
    while (true) {
        system("cls");
        cout << "===========================================" << endl;
        cout << "                   ADMIN                   " << endl;
        cout << "===========================================" << endl;
        cout << "[1] Orderan/Pesanan" << endl;
        cout << "[2] Menu Restoran" << endl;
        cout << "[3] Pengguna Terdaftar" << endl;
        cout << "[0] Exit" << endl;
        cout << "[>] "; cin >> selectUtama;
        if (selectUtama == 1) {
            int nomor;
            while (true){
                system("cls");
                cout << "===========================================" << endl;
                cout << "               DAFTAR ORDERAN              " << endl;
                cout << "===========================================" << endl;
                lihatOrderan(headord);
                cout << "===========================================" << endl;
                cout << "[1] Antar Pesanan Teratas" << endl;
                cout << "[2] Batal Pesanan Teratas" << endl;
                cout << "[0] Kembali" << endl;
                cout << "[>] "; cin >> selectAdmin1;
                if (selectAdmin1 == 1 || selectAdmin1 == 2){
                    orderanSelesai(&headord);
                    NodeKeArrayOrder(&ord[0], headord);
                    ArraykeCSV(&arr[0], &acc[0], &ord[0], 3);
                } else if (selectAdmin1 == 0) {
                    break;
                } else {
                    cout << "Error: Pilihan tidak tersedia." << endl;
                    system("pause");
                }
            }
        } else if (selectUtama == 2) {
            while (true){
                system("cls");
                cout << "===========================================" << endl;
                cout << "                DAFTAR MENU                " << endl;
                cout << "===========================================" << endl;
                lihatMenu(head, 1);
                cout << "===========================================" << endl;
                cout << "                   ADMIN                   " << endl;
                cout << "===========================================" << endl;
                cout << "[1] Tambah Menu" << endl;
                cout << "[2] Edit Menu" << endl;
                cout << "[3] Hapus Menu" << endl;
                cout << "[4] Urut Menu" << endl;
                cout << "[5] Cari Menu" << endl;
                cout << "[0] Kembali" << endl;
                cout << "[>] "; cin >> selectAdmin2;
                switch(selectAdmin2){
                    case 1: tambahMenu(&head, &tail); break;
                    case 2: editMenu(head); break;
                    case 3: hapusMenu(&head, &tail); break;
                    case 4: urutMenu(&arr[0], lastNodeIndex); break;
                    case 5: cariMenu(); break;
                    case 0: break;
                    default: 
                        cout << "ERROR: Pilihan tidak tersedia." << endl; 
                        system("pause"); 
                        break;
                }
                if (menuChanged == 1){
                    NodeKeArrayMenu(&arr[0], head);
                    ArraykeCSV(&arr[0], &acc[0], &ord[0], 1);
                    menuChanged = 0; 
                }
            }
        } else if (selectUtama == 3) {
            while (true){
                system("cls");
                cout << "===========================================" << endl;
                cout << "                DAFTAR USER                " << endl;
                cout << "===========================================" << endl;
                lihatUser(headacc);
                cout << "===========================================" << endl;
                cout << "                   ADMIN                   " << endl;
                cout << "===========================================" << endl;
                cout << "[1] Tambah User" << endl;
                cout << "[2] Edit User" << endl;
                cout << "[3] Hapus User" << endl;
                cout << "[0] Kembali" << endl;
                cout << "[>] "; cin >> selectAdmin3;
                switch(selectAdmin3){
                    case 1: tambahAkun(&headacc); break;
                    case 2: editAkun(headacc); break;
                    case 3: hapusAkun(&headacc); break;
                    case 0: break;
                    default: 
                        cout << "ERROR: Pilihan tidak tersedia." << endl; 
                        system("pause"); 
                        break;
                }
                if (akunChanged == 1){
                    NodeKeArrayAkun(&acc[0], headacc);
                    ArraykeCSV(&arr[0], &acc[0], &ord[0], 2);
                    akunChanged = 0; 
                }
            }
        } else if (selectUtama == 0) {
            break;
        } else {
            cout << "Error: Pilihan tidak tersedia." << endl;
            system("pause");
        }
    }
}

void customer(){
    int selectBuy = -1;
    while (true){
        system("cls");
        cout << "===========================================" << endl;
        cout << "                    MENU                   " << endl;
        cout << "===========================================" << endl;
        lihatMenu(head, 2);
        cout << "[0 ] Keluar" << endl; 
        cout << "===========================================" << endl;
        cout << "Pesan [No] > "; cin >> selectBuy;
        if (selectBuy == 0){
            NodeKeArrayOrder(&ord[0], headord);
            ArraykeCSV(&arr[0], &acc[0], &ord[0], 3);
            break;
        } else {
            pesanMenu(head, &psn[0], selectBuy);
        }
    }
}

void regist(NodeAkun **headacc){
    string nama; char ch;
    NodeAkun *newNode = new NodeAkun();
    system("cls");
    cout << "===========================================" << endl;
    cout << "                  REGISTER                 " << endl;
    cout << "===========================================" << endl;
    cout << "Tekan 0 untuk keluar." << endl;
    cout << "Nama: "; cin.ignore(); getline(cin, nama);
    if (nama == "0") return;
    newNode->account.nama = nama;
    cout << "No. HP: "; getline(cin, newNode->account.no_hp);
    cout << "Alamat: "; getline(cin, newNode->account.alamat);
    cout << "Username: "; cin >> newNode->account.username;
    cout << "Password: "; ch = _getch();
    while (ch != 13){
        newNode->account.password.push_back(ch); 
        if (ch != 8) cout << "*";
        ch = _getch();
    }

	newNode->next = NULL;
	if (*headacc == NULL){
		*headacc = newNode;
	} else {
        newNode->next = NULL;
        NodeAkun *current = *headacc;
        while (current->next != NULL) current = current->next;
        current->next = newNode;
    }
    lastNodeAccount++;
    NodeKeArrayAkun(&acc[0], *headacc);
    ArraykeCSV(&arr[0], &acc[0], &ord[0], 2);
    cout << "===========================================" << endl;
    cout << "          Berhasil Registrasi Akun         "   << endl;
    cout << "===========================================" << endl;
    system("pause");
}

void login(Akun *acc){
    while (true) {
        string user, pass; bool userCocok = false, passCocok = false; char ch;
        system("cls");
        cout << "===========================================" << endl;
        cout << "                   LOGIN                   " << endl;
        cout << "===========================================" << endl;
        cout << "Tekan 0 untuk keluar." << endl;
        cout << "Username: "; cin >> user;
        if (user == "0") return;
        cout << "Password: "; ch = _getch();
        while (ch != 13){
            pass.push_back(ch); 
            if (ch != 8) cout << "*";
            ch = _getch();
        }
        cout << endl;
        if (user == "admin"){
            if (pass == "admin"){
                return admin();
            } else {
                cout << "===========================================" << endl;
                cout << "Error: Username atau password Anda salah."   << endl;
                cout << "===========================================" << endl;
                system("pause");
            }
        } 
        for (int i = 0; i < countArrayAccount; i++){
            if (user == acc[i].username){
                userCocok = true;
                if (pass == acc[i].password){
                    passCocok = true;
                    indexAkun = i;
                    break;
                }
            }
        }
        if (userCocok == true && passCocok == true){
            break;
        } else {
            cout << "===========================================" << endl;
            cout << "Error: Username atau password Anda salah."   << endl;
            cout << "===========================================" << endl;
            system("pause");
        }
    }
    return customer();
}

int main(){
    int choose = -1;
    CSVkeArray(&arr[0], &acc[0], &ord[0]);
    ArrayKeNodeMenu(&arr[0], &head, &tail);
    ArrayKeNodeAkun(&acc[0], &headacc);
    ArrayKeNodeOrder(&ord[0], &headord);
    while (choose != 0){
        system("cls");
        cout << "===========================================" << endl;
        cout << "      Selamat Datang di RestoDelivery      " << endl;
        cout << "===========================================" << endl;
        cout << "[1] Register" << endl;
        cout << "[2] Login" << endl;
        cout << "[0] Keluar" << endl;
        cout << "[>] "; cin >> choose;
        switch (choose) {
            case 1: regist(&headacc); break;
            case 2: login(&acc[0]); break;
            case 0: break;
            default: 
                cout << "ERROR: Pilihan tidak tersedia." << endl; 
                system("pause"); 
                break;
        }
    }
    return 0;
}
