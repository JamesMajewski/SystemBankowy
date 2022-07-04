#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <stdlib.h>
using namespace std;

//MAKS 100 KONT
string numeryKontL[100]; //Lista zawierająca zbiór numerów kont
string haslaL[100]; //Lista zawierająca zbiór haseł do kont
string kwotaL[100]; //Lista zawierają zbiór liczb przedstawiających ilość pieniędzy na koncie
string kwotaM[100]; //Kopia listy "kwotaL"; do lepszego zarządzania danymi

void printFile(string name){ //Drukowanie plików tekstowych; name - 'plik.txt'; Rozwiązanie wzięte z: https://www.w3schools.com/cpp/cpp_files.asp
    string text;
    ifstream MyReadFile(name);

    while (getline (MyReadFile, text)) {
        cout << text << endl;
    }
    MyReadFile.close();
}

void getData(){ //Funkcja przenosząca dane z plików tekstowych do list

    int i = 0; //Liczba porządkowa

    string numeryKont; //Przeniesienie danych od numerów kont
    ifstream numeryKontFile("numeryKont.txt");
    while(getline(numeryKontFile, numeryKont)){
        numeryKontL[i] = numeryKont;
        i++;
    }
    numeryKontFile.close();

    i = 0;

    string hasla; //Przeniesienie danych od haseł
    ifstream haslaFile("hasla.txt");
    while(getline(haslaFile, hasla)){
        haslaL[i] = hasla;
        i++;
    }
    numeryKontFile.close(); 

    i = 0;

    string kwota; //Przeniesienie danych od kwoty
    ifstream kwotaFile("kwota.txt");
    while(getline(kwotaFile, kwota)){
        kwotaL[i] = kwota;
        i++;
    }
    numeryKontFile.close();
    copy(begin(kwotaL), end(kwotaL), begin(kwotaM)); //Kopiowanie listy kwotaL do kwotaM  
}

bool checkExistance(int lp, int zakres, int dane){ //Funckaj do sprawdzania, czy dane konto istnieje; lp - liczba porządkowa, zakres - zakres, dane - input
        getData();
        bool Exists = false;

        while (lp < zakres){
        if (numeryKontL[lp] == to_string(dane)){
            Exists = true;
            break;
        } else {
            lp++;
            continue;
        }
    }
    return Exists;
}

void transfer(int nrKontaT, int nrKontaR, int liczbaKont){ //Funckja odpowiadająca przelewom
    int kwotaD = 0; //Zmienna przechowująca przelewaną kwotę
    int status = 0; //Zmienna służąca do sprawdzenia czy odjęto i dodano kwotę do odpowiendich kąt


    cout << "Prosimy o podanie kwoty do przelania." << endl << "UWAGA! Po wpisaniu liczby, czynnosci nie da sie cofnac." << endl;

    cin >> kwotaD; //Zebranie kwoty do przelewu

    if (kwotaD > stoi(kwotaM[nrKontaT]) || kwotaD < 1){ //Jeżeli kwota jest mniejsza niż stan konta lub kwota jest mniejsza od 1
        cout << "Brak podanych srodkow na koncie lub kwota do przelania wynosi 0." << endl << "Prosimy podac prawidlowa kwote" << endl;
        cin >> kwotaD; //Zebranie na nowo kwoty do przelewu
    } else {

        ofstream transfer;
        transfer.open("kwota.txt");
        int l = 0;


        while(status != 2){ //Sprawdzenie czy przelewy został zrealizowany obustronnie
            string balance = kwotaM[l];
            if (numeryKontL[nrKontaT] == numeryKontL[l]){
                transfer << stoi(balance) - kwotaD << endl;
                status + 1;
                l++;
            } else if (numeryKontL[nrKontaR] == numeryKontL[l]) {
                transfer << stoi(balance) + kwotaD << endl;
                status+1;
                l++;

            } else {
                transfer << balance << endl;
                l++;
            }

        }

        transfer << "." << endl;
        transfer.close();
    }
}



void przelew(int nrKontaDP){ //Podfunkcja do funckji transfer; zbieranie danych - drzewko decyzji

    getData();

    int receiver; //Numer konta odbierającego
    int y = 0; //Liczba porządkowa
    int o = 0; //Lp
    int u = 0; //Lp
    int r = 0; //Lp
    int yKont; //Licznik kont
    int kontynuacja; //Element drzewka decyzyjnego
    int kontoB = 0; //Liczba porządkowa do funckji transfer

    string numeryKont; //Przypisanie ilości kont do zmiennej yKont
    ifstream numeryKontFile("numeryKont.txt");
    while(getline(numeryKontFile, numeryKont)){
        if (numeryKontL[y] == "."){
            yKont = y;
        } else
        numeryKontL[y] = numeryKont;
        y++;
    }
    numeryKontFile.close();

    cout << "Prosze podaj numer konta numer konta, na ktory chce Pan/Pani przelac srodki." << endl;

    cin >> receiver; //Zebranie zmiennej receiver

    while(checkExistance(r, yKont, receiver) != true){ //Sprawdzenie czy konto istnieje
        cout << "Podaj prawidlowy numer konta" << endl;
        cin >> receiver;
        continue;
    }

    while (o < yKont){
        if (to_string(receiver) == numeryKontL[nrKontaDP]){ //Sprawzenie czy konto nie jest to samo co zalogowane
            cout << "Nie mozna przelac pieniedzy na konto, na ktorym jest sie zalogowanym." << endl;
            cout << "Podaj inny numer konta." << endl;
            cin >> receiver;
            while(checkExistance(r, yKont, receiver) != true){ //Sprawdzenie czy konto istnieje
                cout << "Podaj prawidlowy numer konta" << endl;
                cin >> receiver;
                continue;
            }
        } else if (numeryKontL[o] == to_string(receiver)){ //Rozpoczęcie operacji przelania środków
            cout << "Rozpoczynasz operacje przelania srodkow na konto numer: " << receiver << endl;
            cout << "Czy chcesz kontynuowac?" << endl << "1. Tak" << endl << "2. Nie" << endl;
            cin >> kontynuacja; //Zebranie informacji do dzrzewka decyzyjnego

            if (kontynuacja == 1){ 
                
                string numeryKont; //Przypisanie lokalizacji konta otrzymującego zmiennej kontoB
                ifstream numeryKontFile("numeryKont.txt");
                while(getline(numeryKontFile, numeryKont)){
                    if (numeryKontL[u] == to_string(receiver)){
                        kontoB = u;
                    } else
                        numeryKontL[u] = numeryKont;
                        u++;
                    }       
                    numeryKontFile.close();

                transfer(nrKontaDP, kontoB, o);
                break;

            } else {
                continue;
            }
        } else {
            o++;
            continue;
        }
    }


}

void logged(int nrKonta){ //Funkcja po zalogowaniu + drzewko decyzyjne
    getData();
    int anwser;


    cout << endl;
    cout << endl;
    cout << "Dostepne srodki: " << kwotaM[nrKonta] << "zl" << endl; 
    cout << endl;
    cout << "Co chcesz zrobic?" << endl;
    cout << "1. Wyjsc z banku." << endl << "2. Wykonac przelew." << endl;
    cin >> anwser;
    
    if (anwser == 1){
        exit(0);
    } else if (anwser == 2){
        przelew(nrKonta);
    } else {
        cout << "Niepoprawna operacja." << endl;
    }
}

int main(){ //main funckja programu
    getData();

    int givenNr; //Zmienna przechowująca numer konta
    string givenPsw; //Zmienna przechowująca hasło
    int qKont; //Ilośc kont
    int q = 0; //Lp
    int z = 0; //Lp

    printFile("logoBank.txt"); //Drukowanie logo

    string numeryKont; //Przypisanie ilości kont zmiennej qKont
    ifstream numeryKontFile("numeryKont.txt");
    while(getline(numeryKontFile, numeryKont)){
        if (numeryKontL[z] == "."){
            qKont = z;
        } else
        numeryKontL[z] = numeryKont;
        z++;
    }
    numeryKontFile.close();

    cout << endl;
    cout << "Witmay w aplikacji Narodowego Banku Polskiego." << endl;
    cout << "Prosimy o podane numeru konta." << endl;

    cin >> givenNr; //Zebranie numeru konta

    while (givenNr < 0){ //Jeżeli numer konta większy niż 0 + funkcja do logowania
        while (q < qKont){
            if (numeryKontL[q] == to_string(givenNr)){
                cout << "Podaj haslo dla konta: " << numeryKontL[q] << endl;
                cin >> givenPsw;
                if (givenPsw == haslaL[q]){
                    cout << "Pomyslnie zalogowano";
                    logged(q);
                    cout << kwotaM[givenNr];
                    exit(0);

                } else if (givenNr == 0) {
                    exit(0);
                } else {
                    cout << "Nieprawidlowe haslo." << endl;
                    q = 0;
                    continue;
                }
            } else {
                q++;
                continue;
            }

        }
        cout << "Nie znaleziono podanego konta." << endl;
        cout << "Podaj prawidlowy numer, lub wpisz 0, by zakonczyc proces logowania." << endl;
        cin >> givenNr;
    }
 
}
