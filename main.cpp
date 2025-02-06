#include "funkcje.cpp"  // Wczytanie wszystkich funkcji i struktur

int main() {
    // Ustawienie polskich znak�w w konsoli (Windows):
    system("chcp 1250 > nul");

    // Sprawdzamy, czy istnieje domy�lne konto pracownicze (admin/admin):
    sprawdzLubStworzDomyslneKontoPracownicze();

    int wybor = 0;
    do {
        wybor = PrzechwycWybor();
        switch (wybor) {
            case 1:
                logowaniePracownik();
                break;
            case 2:
                logowanieUzytkownik();
                break;
            case 3:
                // Wyj�cie z programu
                break;
            default:
                cout << RED << "Niepoprawna opcja." << RESET << endl;
                break;
        }
    } while(wybor != 3);

    return 0;
}

