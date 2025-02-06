#include "funkcje.cpp"  

int main() {
    
    system("chcp 1250 > nul");

    
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
               
                break;
            default:
                cout << RED << "Niepoprawna opcja." << RESET << endl;
                break;
        }
    } while(wybor != 3);

    return 0;
}

