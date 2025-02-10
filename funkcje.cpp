#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;


const char* RED = "\033[31m";
const char* GREEN = "\033[32m";
const char* YELLOW = "\033[33m";
const char* MAGENTA = "\033[35m";
const char* CYAN = "\033[36m";
const char* BOLD = "\033[1m";
const char* RESET = "\033[0m";



struct Ksiazka {
    char tytul[50];
    char autor[50];
    int ilosc;
    int prog_zamowienia;
    double cenaJednostkowa;
    char gatunek[50];
    int strony;
};

struct Uzytkownik {
    char numerID[10];
    char haslo[10];
    char imieNazwisko[50];
    char adres[50];
};

struct Pracownik {
    char login[50];
    char haslo[20];
    char imieNazwisko[50];
    char adres[50];
};

struct Wypozyczenie {
    char numerID[10];
    char tytul[50];
    char autor[50];
    time_t data_wypozyczenia;
    time_t data_zwrotu;
    double cenaJednostkowa;
};

struct Dostawa {
    char tytul[50];
    char autor[50];
    int ilosc;
    int prog_zamowienia;
    double cenaJednostkowa;
    char gatunek[50];
    int strony;
};


struct LogEntry {
    time_t timestamp;
    char kto[50];
    char opis[200];
};



void utworzKontoUzytkownika();
void utworzKontoPracownika();


void logowaniePracownik();
void logowanieUzytkownik();


void menuPracownika();
void menuKlienta(const char* id);


void usunKsiazke();
void przegladajKsiazki();


void wypozyczKsiazke();
void oddajKsiazke();
void sprawdzWypozyczenia();
void naliczKary();


void logAkcja(const char* kto, const char* opis);
void pokazHistorie();


void rankingNajchetniejWypozyczanych();


void zmienHasloUzytkownika();
void zmienHasloPracownika();


void automatyczneZamawianie();
void reczneZamawianie();
void zatwierdzDostawe();
void zarzadzajDostawami();


void generujHaslo(char *buf, int dlugosc);
void generujID(char *buf, int dlugosc);
int PrzechwycWybor();

bool czyPlikPusty(const char* nazwa);
void pokazUzytkownikow();


int menuKsiazek();
void wyswietlWszystkieKsiazki(ifstream &in);
void wyszukajKsiazkePoTytule(ifstream &in);
void wyszukajKsiazkePoAutorze(ifstream &in);



int menuKsiazek() {
    int wybor;
    cout << CYAN << "\n--- Dostepne ksiazki ---" << RESET << endl;
    cout << "1. Wyswietl wszystkie ksiazki\n";
    cout << "2. Wyszukaj ksiazke po tytule\n";
    cout << "3. Wyszukaj ksiazke po autorze\n";
    cout << "4. Wyszukaj ksiazke po gatunku\n";
    cout << "Wybor: ";
    cin >> wybor;
    cin.ignore();
    return wybor;
}

void wyswietlWszystkieKsiazki(ifstream &in)
{
    cout << CYAN << "\n--- Wszystkie ksiazki ---" << RESET << endl;
    Ksiazka ks;
    in.clear();
    in.seekg(0, ios::beg);
    while (in.read((char*)&ks, sizeof(Ksiazka))) {
        cout << YELLOW << "Tytul: " << RESET << ks.tytul
             << ", Autor: " << ks.autor
             << ", Gatunek: "<< ks.gatunek
             << ", Strony: "<< ks.strony
             << ", Ilosc: " << ks.ilosc
             << ", Prog: " << ks.prog_zamowienia
             << ", Cena: " << ks.cenaJednostkowa << " PLN\n";
    }
}

void wyszukajKsiazkePoTytule(ifstream &in)
{
    in.clear();
    in.seekg(0, ios::beg);
    Ksiazka ks;
    bool znaleziono = false;
    char tytulSzukany[50];
    cout << "Podaj tytul ksiazki do wyszukania: ";
    cin.getline(tytulSzukany, 50);
    while (in.read((char*)&ks, sizeof(Ksiazka))) {
        if (strstr(ks.tytul, tytulSzukany) != nullptr) {
            cout << YELLOW << "Tytul: " << RESET << ks.tytul
                 << ", Autor: " << ks.autor
                 << ", Gatunek: "<< ks.gatunek
                 << ", Strony: "<< ks.strony
                 << ", Ilosc: " << ks.ilosc
                 << ", Prog: " << ks.prog_zamowienia
                 << ", Cena: " << ks.cenaJednostkowa << " PLN\n";
            znaleziono = true;
        }
    }
    if (!znaleziono)
        cout << YELLOW << "Nie znaleziono ksiazki o tytule: "
             << tytulSzukany << RESET << endl;
}

void wyszukajKsiazkePoAutorze(ifstream &in)
{
    in.clear();
    in.seekg(0, ios::beg);
    Ksiazka ks;
    bool znaleziono = false;
    char autorSzukany[50];
    cout << "Podaj autora ksiazki do wyszukania: ";
    cin.getline(autorSzukany, 50);
    while (in.read((char*)&ks, sizeof(Ksiazka))) {
        if (strstr(ks.autor, autorSzukany) != nullptr) {
            cout << YELLOW << "Tytul: " << RESET << ks.tytul
                 << ", Autor: " << ks.autor
                 << ", Gatunek: "<< ks.gatunek
                 << ", Strony: "<< ks.strony
                 << ", Ilosc: " << ks.ilosc
                 << ", Prog: " << ks.prog_zamowienia
                 << ", Cena: " << ks.cenaJednostkowa << " PLN\n";
            znaleziono = true;
        }
    }
    if (!znaleziono)
        cout << YELLOW << "Nie znaleziono ksiazki autora: "
             << autorSzukany << RESET << endl;
}

void wyszukajKsiazkePoGatunku(ifstream &in) {
    in.clear();
    in.seekg(0, ios::beg);

    Ksiazka ks;
    bool znaleziono = false;
    char gatunekSzukany[50];

    cout << "Podaj gatunek ksiazki do wyszukania: ";
    cin.getline(gatunekSzukany, 50);

    while(in.read((char*)&ks, sizeof(Ksiazka))) {

        if(strstr(ks.gatunek, gatunekSzukany) != nullptr) {
            cout << "Tytul: " << ks.tytul
                 << ", Autor: " << ks.autor
                 << ", Gatunek: " << ks.gatunek
                 << ", Strony: " << ks.strony
                 << ", Ilosc: " << ks.ilosc
                 << ", Prog: " << ks.prog_zamowienia
                 << ", Cena: " << ks.cenaJednostkowa << " PLN" << endl;
            znaleziono = true;
        }
    }

    if(!znaleziono) {
        cout << "Nie znaleziono ksiazki o podanym gatunku." << endl;
    }
}


bool czyPlikPusty(const char* nazwa) {
    ifstream f(nazwa, ios::binary);
    if(!f.is_open()) {
        return true;
    }
    f.seekg(0, ios::end);
    streampos rozmiar = f.tellg();
    f.close();
    return (rozmiar == 0);
}

void sprawdzLubStworzDomyslneKontoPracownicze() {
    if(czyPlikPusty("pracownicy.bin")) {
        cout << GREEN << "\nBrak kont w pracownicy.bin. Tworze domyslne konto (admin / admin).\n" << RESET << endl;
        Pracownik p;
        strcpy(p.login, "admin");
        strcpy(p.haslo, "admin");
        ofstream plik("pracownicy.bin", ios::binary);
        if(!plik) {
            cout << "Blad zapisu pracownicy.bin" << endl;
            return;
        }
        plik.write((char*)&p, sizeof(Pracownik));
        plik.close();
    }
}

void generujHaslo(char *buf, int dlugosc) {
    srand((unsigned)time(NULL));
    for(int i=0; i<dlugosc-1; i++) {
        buf[i] = char('0' + rand()%10);
    }
    buf[dlugosc-1] = '\0';
}

void generujID(char *buf, int dlugosc) {
    srand((unsigned)time(NULL));
    int val = rand() % 9000 + 1000;
    snprintf(buf, dlugosc, "%d", val);
}



void logAkcja(const char* kto, const char* opis) {
    ofstream hf("historia.bin", ios::binary | ios::app);
    if(!hf) {
        return;
    }
    LogEntry entry;
    entry.timestamp = time(NULL);
    strncpy(entry.kto, kto, 50);
    entry.kto[49] = '\0';
    strncpy(entry.opis, opis, 200);
    entry.opis[199] = '\0';
    hf.write((char*)&entry, sizeof(LogEntry));
    hf.close();
}

void pokazHistorie() {
    cout << CYAN << "\n--- Historia dzialan ---" << RESET << endl;
    ifstream hf("historia.bin", ios::binary);
    if(!hf) {
        cout << RED << "Brak pliku historia.bin." << RESET << endl;
        return;
    }
    bool pusty = true;
    LogEntry entry;
    while(hf.read((char*)&entry, sizeof(LogEntry))) {
        pusty = false;
        char *dt = ctime(&entry.timestamp);
        if(dt[strlen(dt)-1]=='\n') dt[strlen(dt)-1] = '\0';
        cout << "[" << dt << "] [" << entry.kto << "] " << entry.opis << "\n";
    }
    hf.close();
    if(pusty)
        cout << YELLOW << "Brak dotychczasowych wpisow w historii." << RESET << endl;
}



void rankingNajchetniejWypozyczanych() {
    struct Stat {
        char tytul[50];
        int licznik;
    };

    const int MAX_STAT = 100;
    Stat tab[MAX_STAT];
    for(int i=0; i<MAX_STAT; i++) {
        tab[i].tytul[0] = '\0';
        tab[i].licznik = 0;
    }

    ifstream in("wypozyczenia.bin", ios::binary);
    if(!in) {
        cout << "Brak wypozyczen." << endl;
        return;
    }
    Wypozyczenie w;
    while(in.read((char*)&w, sizeof(Wypozyczenie))) {
        bool found = false;
        for(int i=0; i<MAX_STAT; i++) {
            if(tab[i].tytul[0] == '\0') {
                strcpy(tab[i].tytul, w.tytul);
                tab[i].licznik = 1;
                found = true;
                break;
            } else if(strcmp(tab[i].tytul, w.tytul) == 0) {
                tab[i].licznik++;
                found = true;
                break;
            }
        }
    }
    in.close();

    for(int i=0; i<MAX_STAT-1; i++) {
        for(int j=0; j<MAX_STAT-1-i; j++) {
            if(tab[j].licznik < tab[j+1].licznik) {
                Stat tmp = tab[j];
                tab[j] = tab[j+1];
                tab[j+1] = tmp;
            }
        }
    }

    cout << GREEN << "\nNajchetniej wypozyczane ksiazki:" << RESET << "\n";
    for(int i=0; i<MAX_STAT; i++) {
        if(tab[i].tytul[0] == '\0') break;
        cout << i+1 << ". " << tab[i].tytul << " (" << tab[i].licznik << " wypozyczen)\n";
    }
}



void utworzKontoUzytkownika() {
    cout << "\n--- Tworzenie konta uzytkownika ---" << endl;
    Uzytkownik u;
    generujID(u.numerID, 10);
    generujHaslo(u.haslo, 6);
    cin.ignore();
    cout << "Podaj imie i nazwisko: ";
    cin.getline(u.imieNazwisko, 50);
    cout << "Podaj adres: ";
    cin.getline(u.adres, 50);
    ofstream plik("uzytkownicy.bin", ios::binary | ios::app);
    if(!plik) {
        cout << RED << "Blad otwarcia uzytkownicy.bin" << RESET << endl;
        return;
    }
    plik.write((char*)&u, sizeof(Uzytkownik));
    plik.close();
    cout << GREEN << "Stworzono konto uzytkownika." << RESET << endl;
    cout << CYAN << "Nowe ID: " << u.numerID << RESET << "\n";
    cout << CYAN << "Haslo (automatyczne): " << u.haslo << RESET << "\n";
    logAkcja("PRACOWNIK", "Utworzono konto uzytkownika");
}

void utworzKontoPracownika() {
    cout << "\n--- Tworzenie konta pracownika ---" << endl;
    Pracownik p;
    cin.ignore();
    cout << "Login: ";
    cin.getline(p.login, 50);
    cout << "Haslo: ";
    cin.getline(p.haslo, 20);
    cout << "Podaj imie i nazwisko: ";
    cin.getline(p.imieNazwisko, 50);
    cout << "Podaj adres: ";
    cin.getline(p.adres, 50);

    ofstream plik("pracownicy.bin", ios::binary | ios::app);
    if (!plik) {
        cout << RED << "Blad otwarcia pracownicy.bin" << RESET << endl;
        return;
    }
    plik.write((char*)&p, sizeof(Pracownik));
    plik.close();

    cout << GREEN << "Stworzono konto pracownika." << RESET << endl;
    logAkcja("PRACOWNIK", "Utworzono nowe konto pracownika");
}


void pokazUzytkownikow() {
    cout << CYAN << "\n--- Lista uzytkownikow ---" << RESET << endl;
    ifstream plik("uzytkownicy.bin", ios::binary);
    if(!plik) {
        cout << RED << "Brak pliku uzytkownicy.bin" << RESET << endl;
        return;
    }
    Uzytkownik u;
    bool pusty = true;
    while(plik.read((char*)&u, sizeof(Uzytkownik))) {
        pusty = false;
        cout << YELLOW << "ID: " << RESET << u.numerID << "\n";
        cout << YELLOW <<"Imie i nazwisko: " << RESET << u.imieNazwisko << "\n";
        cout << YELLOW <<"Adres: " << RESET << u.adres << "\n";
        cout << "------------------------------\n";
    }
    plik.close();
    if(pusty)
        cout << YELLOW << "Brak zarejestrowanych uzytkownikow." << RESET << endl;
}



void menuPracownika();
void menuKlienta(const char* id);

void logowaniePracownik() {
    cout << BOLD << "\n--- Logowanie pracownika ---" << RESET << endl;
    char login[50], haslo[20];
    cin.ignore();
    cout << "Login: ";
    cin.getline(login, 50);
    cout << "Haslo: ";
    cin.getline(haslo, 20);

    ifstream plik("pracownicy.bin", ios::binary);
    if (!plik) {
        cout << RED << "Brak pliku pracownicy.bin lub blad otwarcia." << RESET << endl;
        return;
    }

    Pracownik p;
    bool zalogowano = false;
    while (plik.read((char*)&p, sizeof(Pracownik))) {

        if (strcmp(p.login, login) == 0 && strcmp(p.haslo, haslo) == 0) {
            zalogowano = true;
            break;
        }
    }
    plik.close();

    if (zalogowano) {
        cout << GREEN << "Zalogowano pracownika: " << login << RESET << "\n";
        cout << YELLOW << "Zyczymy milej pracy!" << RESET << "\n";
        logAkcja(login, "Logowanie pracownika");
        menuPracownika();
    } else {
        cout << RED << "Niepoprawne dane logowania." << RESET << endl;
    }
}


void logowanieUzytkownik() {
    cout << BOLD << "\n--- Logowanie uzytkownika ---" << RESET << endl;
    char numerID[10], pass[10];
    cin.ignore();
    cout << "Numer ID: ";
    cin.getline(numerID, 10);
    cout << "Haslo: ";
    cin.getline(pass, 10);
    ifstream plik("uzytkownicy.bin", ios::binary);
    if(!plik) {
        cout << RED << "Brak pliku uzytkownicy.bin lub blad otwarcia." << RESET << endl;
        return;
    }
    Uzytkownik u;
    bool zalogowano = false;
    while(plik.read((char*)&u, sizeof(Uzytkownik))) {
        if(strcmp(u.numerID, numerID)==0 && strcmp(u.haslo, pass)==0) {
            zalogowano = true;
            break;
        }
    }
    plik.close();
    if(zalogowano) {
        cout << GREEN << "Zalogowano uzytkownika: " << numerID << RESET << "\n";
        cout << YELLOW << "Milego czytania!" << RESET << "\n";
        logAkcja(numerID, "Logowanie uzytkownika");
        menuKlienta(numerID);
    } else
        cout << RED << "Niepoprawne dane logowania." << RESET << endl;
}



void zmienHasloPracownika() {
    cout << CYAN << "\n--- Zmiana hasla pracownika ---" << RESET << endl;
    char login[50], stare[20], nowe[20];
    cin.ignore();
    cout << CYAN << "Podaj login: " << RESET;
    cin.getline(login, 50);
    cout << CYAN << "Podaj stare haslo: " << RESET;
    cin.getline(stare, 20);

    fstream plik("pracownicy.bin", ios::binary | ios::in | ios::out);
    if(!plik) {
        cout << RED << "Blad otwarcia pliku pracownicy.bin." << RESET << endl;
        return;
    }
    Pracownik p;
    bool zmieniono = false;
    streampos pos;

    while(true) {
        pos = plik.tellg();
        if(!plik.read(reinterpret_cast<char*>(&p), sizeof(Pracownik)))
            break;
        if(strcmp(p.login, login) == 0 && strcmp(p.haslo, stare) == 0) {
            cout << CYAN << "Podaj nowe haslo: " << RESET;
            cin.getline(nowe, 20);
            strcpy(p.haslo, nowe);
            plik.seekp(pos);
            plik.write(reinterpret_cast<char*>(&p), sizeof(Pracownik));
            cout << GREEN << "Haslo zostalo zmienione." << RESET << endl;
            zmieniono = true;
            break;
        }
    }
    plik.close();
    if(!zmieniono)
        cout << RED << "Nie udalo sie zmienic hasla. Bledne dane." << RESET << endl;
}

void menuPracownika() {
    int wybor;
    do {
        cout << CYAN << "\n==== MENU PRACOWNIKA ====" << RESET << endl;
        cout << "1. Usun ksiazke\n";
        cout << "2. Wypozycz ksiazke\n";
        cout << "3. Oddaj ksiazke\n";
        cout << "4. Sprawdz wypozyczenia\n";
        cout << "5. Nalicz kary\n";
        cout << "6. Zarzadzaj dostawami\n";
        cout << "7. Stworz konto uzytkownika\n";
        cout << "8. Stworz konto pracownika\n";
        cout << "9. Przegladaj ksiazki\n";
        cout << "10. Wyswietl uzytkownikow\n";
        cout << "11. Pokaz historie dzialan\n";
        cout << "12. Zmien haslo pracownika\n";
        cout << "13. Wyloguj\n";
        cout << "Wybor: ";
        cin >> wybor;
        switch(wybor) {
            case 1:
                usunKsiazke();
                break;
            case 2:
                wypozyczKsiazke();
                break;
            case 3:
                oddajKsiazke();
                break;
            case 4:
                sprawdzWypozyczenia();
                break;
            case 5:
                naliczKary();
                break;
            case 6:
                zarzadzajDostawami();
                break;
            case 7:
                utworzKontoUzytkownika();
                break;
            case 8:
                utworzKontoPracownika();
                break;
            case 9:
                przegladajKsiazki();
                break;
            case 10:
                pokazUzytkownikow();
                break;
            case 11:
                pokazHistorie();
                break;
            case 12:
                zmienHasloPracownika();
                break;
            case 13:
                cout << RED << "Wylogowano pracownika." << RESET << endl;
                break;
            default:
                cout << RED << "Niepoprawny wybor." << RESET << endl;
        }
    } while(wybor != 13);
}


void menuKlientaRec(const char* id) {
    int wybor;
    cout << CYAN << "\n==== MENU KLIENTA ====" << RESET << endl;
    cout << "1. Przegladaj swoje wypozyczenia\n";
    cout << "2. Sprawdz ile czasu zostalo do zwrotu\n";
    cout << "3. Zmien haslo\n";
    cout << "4. Przegladaj ksiazki\n";
    cout << "5. Zestawienie najchetniej wypozyczanych\n";
    cout << "6. Wyloguj\n";
    cout << "Wybor: ";
    cin >> wybor;
    switch(wybor) {
        case 1:
        {
            ifstream in("wypozyczenia.bin", ios::binary);
            if(!in) {
                cout << RED << "Brak wypozyczen." << RESET << endl;
            } else {
                Wypozyczenie w;
                bool znaleziono = false;
                while(in.read((char*)&w, sizeof(Wypozyczenie))) {
                    if(strcmp(w.numerID, id) == 0 && w.data_zwrotu == 0) {
                        cout << "Tytul: " << w.tytul << ", wypozyczono: " << ctime(&w.data_wypozyczenia);
                        znaleziono = true;
                    }
                }
                in.close();
                if(!znaleziono)
                    cout << YELLOW << "Brak aktywnych wypozyczen." << RESET << endl;
            }
        }
            break;
        case 2:
        {
            ifstream in("wypozyczenia.bin", ios::binary);
            if(!in) {
                cout << RED << "Brak wypozyczen." << RESET << endl;
            } else {
                Wypozyczenie w;
                bool znaleziono = false;
                time_t now = time(NULL);
                while(in.read((char*)&w, sizeof(Wypozyczenie))) {
                    if(strcmp(w.numerID, id) == 0 && w.data_zwrotu == 0) {
                        int dniMinelo = (int)((now - w.data_wypozyczenia) / (60*60*24));
                        int dniPozostale = 30 - dniMinelo;
                        cout << "Tytul: " << w.tytul << ", minelo dni: " << dniMinelo;
                        if(dniPozostale > 0)
                            cout << ", zostalo " << dniPozostale << " dni do zwrotu." << endl;
                        else
                            cout << ", ksiazka jest juz po terminie!" << endl;
                        znaleziono = true;
                    }
                }
                in.close();
                if(!znaleziono)
                    cout << YELLOW << "Brak aktywnych wypozyczen." << RESET << endl;
            }
        }
            break;
        case 3:
            zmienHasloUzytkownika();
            break;
        case 4:
            przegladajKsiazki();
            break;
        case 5:
            rankingNajchetniejWypozyczanych();
            break;
        case 6:
            cout << RED << "Wylogowano uzytkownika." << RESET << endl;
            return;
        default:
            cout << RED << "Niepoprawny wybor." << RESET << endl;
            break;
    }
    menuKlientaRec(id);
}

void menuKlienta(const char* id) {
    menuKlientaRec(id);
}



void usunKsiazke() {
    cout << YELLOW << "\n--- Usuwanie ksiazki ---" << RESET << endl;
    char tytulSzukany[50];
    cin.ignore();
    cout << "Podaj tytul ksiazki do usuniecia (lub czesciowego usuniecia): ";
    cin.getline(tytulSzukany, 50);
    ifstream in("ksiazki.bin", ios::binary);
    if(!in) {
        cout << RED << "Brak pliku ksiazki.bin lub blad otwarcia." << RESET << endl;
        return;
    }
    ofstream out("temp.bin", ios::binary);
    if(!out) {
        cout << RED << "Blad tworzenia pliku tymczasowego." << RESET << endl;
        in.close();
        return;
    }
    bool znaleziono = false;
    Ksiazka ks;
    while(in.read((char*)&ks, sizeof(Ksiazka))) {
        if(strcmp(ks.tytul, tytulSzukany)==0) {
            znaleziono = true;
            cout << "Znaleziono ksiazke '" << ks.tytul << "' Autor: " << ks.autor
                 << ", Ilosc: " << ks.ilosc << ", Cena: " << ks.cenaJednostkowa << "\n";
            cout << "Ile sztuk chcesz usunac? (Obecnie " << ks.ilosc << ") ";
            int ileDoUsuniecia;
            cin >> ileDoUsuniecia;
            if(ileDoUsuniecia >= ks.ilosc) {
                cout << "Usuwam calkowicie ksiazke: " << ks.tytul << "\n";
                logAkcja("PRACOWNIK", "Usunieto calkowicie ksiazke");

            } else if(ileDoUsuniecia > 0) {
                ks.ilosc -= ileDoUsuniecia;
                cout << "Pozostaje " << ks.ilosc << " sztuk ksiazki '" << ks.tytul << "'\n";
                out.write((char*)&ks, sizeof(Ksiazka));
                logAkcja("PRACOWNIK", "Czesciowo usunieto ksiazki");
            } else
                out.write((char*)&ks, sizeof(Ksiazka));
        } else
            out.write((char*)&ks, sizeof(Ksiazka));
    }
    in.close();
    out.close();
    remove("ksiazki.bin");
    rename("temp.bin", "ksiazki.bin");
    if(znaleziono) {
        cout << GREEN << "Operacja usuwania zakonczona." << RESET << endl;
        automatyczneZamawianie();
    } else
        cout << RED << "Nie znaleziono ksiazki o tytule: " << tytulSzukany << RESET << "\n";
}

void wypozyczKsiazke() {
    cout << GREEN << "\n--- Wypozyczanie ksiazki ---" << RESET << endl;
    char numerID[10];
    char tytul[50];
    char autor[50];
    cin.ignore();
    cout << "Podaj numer ID uzytkownika: ";
    cin.getline(numerID, 10);
    cout << "Podaj tytul ksiazki: ";
    cin.getline(tytul, 50);
    cout << "Podaj autora ksiazki: ";
    cin.getline(autor, 50);

    bool idFound = false;
    Uzytkownik tempUser;
    ifstream userFile("uzytkownicy.bin", ios::binary);
    if (!userFile) {
        cout << RED << "Blad otwarcia pliku uzytkownicy.bin." << RESET << endl;
        return;
    }
    while(userFile.read(reinterpret_cast<char*>(&tempUser), sizeof(Uzytkownik))) {
        if(strcmp(tempUser.numerID, numerID) == 0) {
            idFound = true;
            break;
        }
    }
    userFile.close();
    if(!idFound) {
        cout << RED << "Nie znaleziono uzytkownika o numerze ID: " << numerID << RESET << endl;
        return;
    }
    fstream plikKs("ksiazki.bin", ios::binary | ios::in | ios::out);
    if(!plikKs) {
        cout << RED << "Brak pliku ksiazki.bin." << RESET << endl;
        return;
    }
    Ksiazka ks;
    bool znaleziono = false;
    streampos pozycja;
    while(true) {
        pozycja = plikKs.tellg();
        if(!plikKs.read(reinterpret_cast<char*>(&ks), sizeof(Ksiazka)))
            break;

        if(strcmp(ks.tytul, tytul)==0 && strcmp(ks.autor, autor)==0) {
            znaleziono = true;
            if(ks.ilosc > 0) {
                ks.ilosc -= 1;
                plikKs.seekp(pozycja);
                plikKs.write(reinterpret_cast<char*>(&ks), sizeof(Ksiazka));

                Wypozyczenie w;
                strcpy(w.numerID, numerID);
                strcpy(w.tytul, tytul);
                strcpy(w.autor, autor);
                w.data_wypozyczenia = time(NULL);
                w.data_zwrotu = 0;
                w.cenaJednostkowa = ks.cenaJednostkowa;
                ofstream wpl("wypozyczenia.bin", ios::binary | ios::app);
                if(!wpl) {
                    cout << RED << "Blad otwarcia wypozyczenia.bin" << RESET << endl;
                    plikKs.close();
                    return;
                }
                wpl.write(reinterpret_cast<char*>(&w), sizeof(Wypozyczenie));
                wpl.close();
                cout << GREEN << "Wypozyczono ksiazke." << RESET << endl;
                logAkcja("PRACOWNIK", "Wypozyczono ksiazke");
                if(ks.ilosc < ks.prog_zamowienia) {
                    automatyczneZamawianie();
                }
            } else {
                cout << RED << "Brak dostepnych egzemplarzy." << RESET << endl;
            }
            break;
        }
    }
    plikKs.close();
    if(!znaleziono)
        cout << RED << "Nie znaleziono ksiazki o tytule: " << tytul << RESET << "\n";
}


void oddajKsiazke() {
    cout << GREEN << "\n--- Oddawanie ksiazki ---" << RESET << endl;
    char numerID[10];
    char tytul[50];
    char autor[50];
    cin.ignore();
    cout << "Podaj numer ID uzytkownika: ";
    cin.getline(numerID, 10);
    cout << "Podaj tytul ksiazki: ";
    cin.getline(tytul, 50);
    cout << "Podaj autora ksiazki: ";
    cin.getline(autor, 50);

    ifstream in("wypozyczenia.bin", ios::binary);
    if(!in) {
        cout << RED << "Brak wypozyczen." << RESET << endl;
        return;
    }
    ofstream out("temp.bin", ios::binary);
    if(!out) {
        cout << RED << "Blad tworzenia pliku tymczasowego." << RESET << endl;
        in.close();
        return;
    }
    bool znaleziono = false;
    double totalPenalty = 0.0;
    Wypozyczenie w;
    while(in.read((char*)&w, sizeof(Wypozyczenie))) {

        if(strcmp(w.numerID, numerID) == 0 &&
           strcmp(w.tytul, tytul) == 0 &&
           strcmp(w.autor, autor) == 0 &&
           w.data_zwrotu == 0) {
            char stan;
            cout << "Czy ksiazka zostala zwrocona w dobrym stanie? (T/N): ";
            cin >> stan;
            double conditionPenalty = 0.0;
            if(stan == 'N' || stan == 'n') {
                conditionPenalty = w.cenaJednostkowa;
            }
            w.data_zwrotu = time(NULL);
            int dniMinelo = (int)((w.data_zwrotu - w.data_wypozyczenia) / (60 * 60 * 24));
            double delayPenalty = 0.0;
            if(dniMinelo > 30) {
                delayPenalty = (dniMinelo - 30) * 0.20;
            }
            totalPenalty = delayPenalty + conditionPenalty;
            cout << RED << "Kara: " << totalPenalty << " PLN." << RESET << endl;
            znaleziono = true;
        }
        out.write((char*)&w, sizeof(Wypozyczenie));
    }
    in.close();
    out.close();
    if(!znaleziono) {
        cout << RED << "Nie znaleziono aktywnego wypozyczenia." << RESET << endl;
        remove("temp.bin");
        return;
    } else {
        remove("wypozyczenia.bin");
        rename("temp.bin", "wypozyczenia.bin");
        cout << GREEN << "Ksiazka zostala oddana." << RESET << endl;
        logAkcja("PRACOWNIK", "Oddano ksiazke");
    }
    fstream plikKs("ksiazki.bin", ios::binary | ios::in | ios::out);
    if(!plikKs) {
        cout << RED << "Blad otwarcia ksiazki.bin" << RESET << endl;
        return;
    }
    Ksiazka k;
    streampos pos;
    while(true) {
        pos = plikKs.tellg();
        if(!plikKs.read((char*)&k, sizeof(Ksiazka))) break;
        if(strcmp(k.tytul, tytul) == 0 && strcmp(k.autor, autor) == 0) {
            k.ilosc += 1;
            plikKs.seekp(pos);
            plikKs.write((char*)&k, sizeof(Ksiazka));
            break;
        }
    }
    plikKs.close();
}


void sprawdzWypozyczenia() {
    cout << CYAN << "\n--- Aktywne wypozyczenia ---" << RESET << endl;
    ifstream in("wypozyczenia.bin", ios::binary);
    if(!in) {
        cout << RED << "Brak pliku wypozyczenia.bin." << RESET << endl;
        return;
    }
    Wypozyczenie w;
    bool pusto = true;
    while(in.read((char*)&w, sizeof(Wypozyczenie))) {
        if(w.data_zwrotu == 0) {
            pusto = false;
            cout << YELLOW << "Uzytkownik: " << RESET << w.numerID
                 << ", Tytul: " << w.tytul
                 << ", Autor: " << w.autor << "\n"
                 << "Data wypozyczenia: " << ctime(&w.data_wypozyczenia)
                 << ", CenaJednostkowa: " << w.cenaJednostkowa << " PLN\n";
        }
    }
    in.close();
    if(pusto)
        cout << YELLOW << "Brak aktywnych wypozyczen." << RESET << endl;
}


void naliczKary() {
    cout << CYAN << "\n--- Naliczanie kar za przetrzymanie powyzej 30 dni ---" << RESET << endl;
    ifstream in("wypozyczenia.bin", ios::binary);
    if(!in) {
        cout << RED << "Brak wypozyczen." << RESET << endl;
        return;
    }
    bool ktos = false;
    time_t teraz = time(NULL);
    Wypozyczenie w;
    while(in.read((char*)&w, sizeof(Wypozyczenie))) {
        if(w.data_zwrotu == 0) {
            int dni = (int)((teraz - w.data_wypozyczenia)/(60*60*24));
            if(dni > 30) {
                double kara = (dni - 30) * 0.20;
                cout << RED << "Uzytkownik " << w.numerID << " ksiazka '" << w.tytul << "' "
                     << "przetrzymana " << (dni - 30) << " dni, kara = " << kara << " PLN." << RESET << endl;
                ktos = true;
            }
        }
    }
    in.close();
    if(!ktos)
        cout << YELLOW << "Brak kar do naliczenia (za przetrzymanie)." << RESET << endl;
}

void zmienHasloUzytkownika() {
    cout << CYAN << "\n--- Zmiana hasla uzytkownika ---" << RESET << endl;
    char id[10], stare[10], nowe[10];
    cin.ignore();
    cout << CYAN << "Podaj numer ID: " << RESET;
    cin.getline(id, 10);
    cout << CYAN << "Podaj stare haslo: " << RESET;
    cin.getline(stare, 10);
    fstream plik("uzytkownicy.bin", ios::binary | ios::in | ios::out);
    if(!plik) {
        cout << RED << "Brak pliku uzytkownicy.bin." << RESET << endl;
        return;
    }
    Uzytkownik u;
    bool zmieniono = false;
    streampos pos;
    while(true) {
        pos = plik.tellg();
        if(!plik.read((char*)&u, sizeof(Uzytkownik))) break;
        if(strcmp(u.numerID, id)==0 && strcmp(u.haslo, stare)==0) {
            cout << CYAN << "Podaj nowe haslo: " << RESET;
            cin.getline(nowe, 10);
            strcpy(u.haslo, nowe);
            plik.seekp(pos);
            plik.write((char*)&u, sizeof(Uzytkownik));
            cout << GREEN << "Haslo zostalo zmienione." << RESET << endl;
            zmieniono = true;
            break;
        }
    }
    plik.close();
    if(!zmieniono)
        cout << RED << "Nie udalo sie zmienic hasla. Bledne ID lub stare haslo." << RESET << endl;
}





void zarzadzajDostawami() {
    cout << MAGENTA << "\n--- Zarzadzanie dostawami ---" << RESET << endl;
    int wybor;
    do {
        cout << "1. Wyswietl w_dostawie.bin\n";
        cout << "2. Reczne zamawianie\n";
        cout << "3. Zatwierdz dostawe\n";
        cout << "4. Powrot\n";
        cout << "Wybor: ";
        cin >> wybor;
        switch(wybor) {
            case 1:
            {
                ifstream in("w_dostawie.bin", ios::binary);
                if(!in) {
                    cout << RED << "Brak pliku w_dostawie.bin." << RESET << endl;
                    break;
                }
                Dostawa d;
                bool cos = false;
                while(in.read((char*)&d, sizeof(Dostawa))) {
                    cos = true;
                    cout << GREEN << "Tytul: " << RESET << d.tytul << ", Autor: " << d.autor
                         << ", Ilosc: " << d.ilosc
                         << ", Prog: " << d.prog_zamowienia
                         << ", Cena: " << d.cenaJednostkowa << " PLN\n";
                }
                in.close();
                if(!cos) cout << YELLOW << "Brak pozycji w dostawie." << RESET << endl;
            }
            break;
            case 2:
                reczneZamawianie();
                break;
            case 3:
                zatwierdzDostawe();
                break;
            case 4:
                break;
            default:
                cout << RED << "Niepoprawny wybor." << RESET << endl;
        }
    } while(wybor != 4);
}

void automatyczneZamawianie() {
    cout << YELLOW << "\n--- Automatyczne zamawianie ---" << RESET << endl;
    fstream plikKs("ksiazki.bin", ios::binary | ios::in);
    if (!plikKs) {
        cout << RED << "Brak pliku ksiazki.bin." << RESET << endl;
        return;
    }
    Ksiazka ks;
    while (plikKs.read((char*)&ks, sizeof(Ksiazka))) {
        if (ks.ilosc < ks.prog_zamowienia) {
            Dostawa newDostawa;
            strcpy(newDostawa.tytul, ks.tytul);
            strcpy(newDostawa.autor, ks.autor);
            strcpy(newDostawa.gatunek, ks.gatunek);
            newDostawa.strony = ks.strony;
            newDostawa.ilosc = 10;
            newDostawa.prog_zamowienia = ks.prog_zamowienia;
            newDostawa.cenaJednostkowa = ks.cenaJednostkowa;

            fstream dostawy("w_dostawie.bin", ios::binary | ios::in | ios::out);
            if (!dostawy) {
                ofstream createFile("w_dostawie.bin", ios::binary);
                createFile.close();
                dostawy.open("w_dostawie.bin", ios::binary | ios::in | ios::out);
                if (!dostawy) {
                    cout << RED << "Blad otwarcia w_dostawie.bin." << RESET << endl;
                    continue;
                }
            }
            bool found = false;
            Dostawa d;
            dostawy.seekg(0, ios::beg);
            while (dostawy.read((char*)&d, sizeof(Dostawa))) {

                if (strcmp(d.tytul, newDostawa.tytul) == 0 &&
                    strcmp(d.autor, newDostawa.autor) == 0 &&
                    strcmp(d.gatunek, newDostawa.gatunek) == 0 &&
                    d.strony == newDostawa.strony) {
                    d.ilosc += newDostawa.ilosc;
                    dostawy.seekp(-static_cast<streamoff>(sizeof(Dostawa)), ios::cur);
                    dostawy.write((char*)&d, sizeof(Dostawa));
                    found = true;
                    break;
                }
            }
            if (!found) {
                dostawy.clear();
                dostawy.seekp(0, ios::end);
                dostawy.write((char*)&newDostawa, sizeof(Dostawa));
            }
            dostawy.close();
            cout << YELLOW << "Dodano do w_dostawie ksiazke: " << ks.tytul
                 << ", bo ilosc < prog." << RESET << endl;
            logAkcja("SYSTEM", "Automatyczne zamowienie ksiazki");
        }
    }
    plikKs.close();
}






void reczneZamawianie() {
    cout << GREEN << "\n--- Reczne zamawianie ksiazek ---" << RESET << endl;
    Dostawa d;

    cin.ignore();
    cout << "Tytul: ";
    cin.getline(d.tytul, 50);
    cout << "Autor: ";
    cin.getline(d.autor, 50);
    cout << "Ilosc sztuk: ";
    cin >> d.ilosc;
    cout << "Prog zamowienia: ";
    cin >> d.prog_zamowienia;
    cout << "Cena jednostkowa (PLN): ";
    cin >> d.cenaJednostkowa;
    cin.ignore();


    cout << "Gatunek: ";
    cin.getline(d.gatunek, 50);
    cout << "Ilosc stron: ";
    cin >> d.strony;
    cin.ignore();


    fstream dostawy("w_dostawie.bin", ios::binary | ios::in | ios::out);
    if (!dostawy) {
        ofstream createFile("w_dostawie.bin", ios::binary);
        createFile.close();
        dostawy.open("w_dostawie.bin", ios::binary | ios::in | ios::out);
        if (!dostawy) {
            cout << RED << "Blad otwarcia w_dostawie.bin" << RESET << endl;
            return;
        }
    }
    bool found = false;
    Dostawa existing;
    dostawy.seekg(0, ios::beg);
    while (dostawy.read((char*)&existing, sizeof(Dostawa))) {

        if (strcmp(existing.tytul, d.tytul) == 0 &&
            strcmp(existing.autor, d.autor) == 0 &&
            strcmp(existing.gatunek, d.gatunek) == 0 &&
            existing.cenaJednostkowa == d.cenaJednostkowa &&
            existing.strony == d.strony) {
            existing.ilosc += d.ilosc;
            dostawy.seekp(-static_cast<streamoff>(sizeof(Dostawa)), ios::cur);
            dostawy.write((char*)&existing, sizeof(Dostawa));
            found = true;
            break;
        }
    }
    if (!found) {
        dostawy.clear();
        dostawy.seekp(0, ios::end);
        dostawy.write((char*)&d, sizeof(Dostawa));
    }
    dostawy.close();
    cout << GREEN << "Zamowienie zostalo dodane do w_dostawie.bin" << RESET << endl;
    logAkcja("PRACOWNIK", "Reczne zamawianie ksiazek");
}


void przegladajKsiazki() {
    int wybor = menuKsiazek();
    ifstream in("ksiazki.bin", ios::binary);
    if (!in) {
        cout << RED << "Brak pliku ksiazki.bin lub blad otwarcia." << RESET << endl;
        return;
    }
    switch (wybor) {
        case 1:
            wyswietlWszystkieKsiazki(in);
            break;
        case 2:
            wyszukajKsiazkePoTytule(in);
            break;
        case 3:
            wyszukajKsiazkePoAutorze(in);
            break;
        case 4:
            wyszukajKsiazkePoGatunku(in);
            break;
        default:
            cout << RED << "Niepoprawny wybor." << RESET << endl;
            break;
    }
    in.close();
}

void zatwierdzDostawe() {
    cout << GREEN << "\n--- Zatwierdzanie dostawy ---" << RESET << endl;
    ifstream in("w_dostawie.bin", ios::binary);
    if(!in) {
        cout << RED << "Brak pliku w_dostawie.bin." << RESET << endl;
        return;
    }
    fstream ksFile("ksiazki.bin", ios::binary | ios::in | ios::out);
    if(!ksFile) {
        ofstream createKs("ksiazki.bin", ios::binary);
        createKs.close();
        ksFile.open("ksiazki.bin", ios::binary | ios::in | ios::out);
        if(!ksFile) {
            cout << RED << "Blad otwarcia ksiazki.bin" << RESET << endl;
            in.close();
            return;
        }
    }
    ofstream temp("temp_dostawy.bin", ios::binary);
    if(!temp) {
        cout << RED << "Blad tworzenia pliku tymczasowego." << RESET << endl;
        in.close();
        ksFile.close();
        return;
    }
    cout << "Czy chcesz zatwierdzic wszystkie dostawy naraz? (T/N): ";
    char opcjaAll;
    cin >> opcjaAll;
    bool cos = false;
    Dostawa d;
    while(in.read((char*)&d, sizeof(Dostawa))) {
        bool znaleziono = false;
        Ksiazka ks;
        ksFile.seekg(0, ios::beg);
        while(ksFile.read((char*)&ks, sizeof(Ksiazka))) {

            if(strcmp(ks.tytul, d.tytul) == 0 &&
               strcmp(ks.autor, d.autor) == 0 &&
               strcmp(ks.gatunek, d.gatunek) == 0 &&
               ks.strony == d.strony &&
               ks.cenaJednostkowa == d.cenaJednostkowa) {
                streampos pos = ksFile.tellg();
                pos -= sizeof(Ksiazka);
                ksFile.seekp(pos);
                ks.ilosc += d.ilosc;
                ks.prog_zamowienia = d.prog_zamowienia;

                ksFile.write((char*)&ks, sizeof(Ksiazka));
                znaleziono = true;
                cos = true;
                cout << CYAN << "Zaktualizowano ksiazke: " << RESET << ks.tytul
                     << " (gatunek: " << ks.gatunek << ", strony: " << ks.strony
                     << "), nowa ilosc: " << ks.ilosc << endl;
                break;
            }
        }
        if(!znaleziono) {
            Ksiazka nowa;
            strcpy(nowa.tytul, d.tytul);
            strcpy(nowa.autor, d.autor);
            nowa.ilosc = d.ilosc;
            nowa.prog_zamowienia = d.prog_zamowienia;
            nowa.cenaJednostkowa = d.cenaJednostkowa;

            strcpy(nowa.gatunek, d.gatunek);
            nowa.strony = d.strony;
            ksFile.clear();
            ksFile.seekp(0, ios::end);
            ksFile.write((char*)&nowa, sizeof(Ksiazka));
            cos = true;
            cout << CYAN << "Dodano nowa ksiazke: " << RESET << nowa.tytul
                 << " (gatunek: " << nowa.gatunek << ", strony: " << nowa.strony
                 << "), ilosc: " << nowa.ilosc << endl;
        }
        if(opcjaAll != 'T' && opcjaAll != 't') {
            cout << "Zatwierdzic te pozycje? (T/N): ";
            char decyzja;
            cin >> decyzja;
            if(decyzja == 'N' || decyzja == 'n') {
                temp.write((char*)&d, sizeof(Dostawa));
                cos = false;
            }
        }
    }
    in.close();
    temp.close();
    ksFile.close();
    remove("w_dostawie.bin");
    rename("temp_dostawy.bin", "w_dostawie.bin");
    if(cos) {
        cout << GREEN << "Dostawa zostala zatwierdzona." << RESET << endl;
        logAkcja("PRACOWNIK", "Zatwierdzono dostawe bez duplikatow");
    } else
        cout << YELLOW << "Brak zmian do zatwierdzenia." << RESET << endl;
}

int PrzechwycWybor(){
    int wybor;
    cout << YELLOW << "\n========== MENU GLOWNE ==========" << RESET << endl;
    cout << BOLD<< "1." << RESET << " Logowanie jako pracownik" << endl;
    cout << BOLD << "2." << RESET << " Logowanie jako klient" << endl;
    cout << BOLD<< "3." << RESET << " Wyjscie" << endl;
    cout << "Wybor: ";
    cin >> wybor;
    return wybor;
}
