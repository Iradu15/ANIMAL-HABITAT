#include <iostream>
#include <vector>
#include <algorithm> 
#include <string.h>
#include <unordered_map>
#include <stack>
#include <fstream>
#include <deque>
#include <queue>
#include <csignal>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include <random>
using namespace std;

ifstream fin("a.txt");

int MaxAge = 5;                                                   //la alegerea progrmatorrului aceste constante
double constGivingBirth = 0.2;
double constGettingOld = 0.125;
double constInitEnergy = 10;


const int nrR = 3;                                                 //dimensiunile tablei(70-20 in cerinta)
const int nrC = 3;

class Individ {
    int i, j;
    char tip;
    int varsta;
    double energie;
    unsigned char viu;

    //metode private
    void feed(Individ**&);
    void giveBirth(Individ**&);
    void attack(Individ**&);
    void getsOld();
    void die();

public:
    Individ(int, int, char);
    Individ();
    void update(Individ**&);
    unsigned char isAlive();
    char gettype();

    void addOnTable(int, int, char, Individ**&);
    friend ostream& operator<<(ostream& os, Individ& mat);
    void afisare(Individ**&);
};

//constructori
Individ::Individ() {
    i = 0;
    j = 0;
    tip = '0';
    varsta = 0;
    energie = 0; // valoare constanta aleasa de programator
    viu = '0';

    //trebuie aplicata functia alocare de la tabelSpecii
};

Individ::Individ(int pi, int pj, char type) {
    i = pi;
    j = pj;
    varsta = 0;
    energie = constInitEnergy; // valoare constanta aleasa de programator
    viu = '1';
    tip = type;
    //trebuie aplicata functia alocare de la tabelSpecii
};

//metode private

void Individ::feed(Individ**& mat) {
    int aportEnergetic = 1;
    int x = i, y = j;
    while (++y < nrC && mat[x][y].isAlive() == '0')
        aportEnergetic++;
    y = j;                                                                  // calculeaza spatiile libere din apropiere
    while (--y > 0 && mat[x][y].isAlive() == '0')
        aportEnergetic++;

    x = i, y = j;
    while (++x < nrR && mat[x][y].isAlive() == '0')
        aportEnergetic++;
    x = i;
    while (--x > 0 && mat[x][y].isAlive() == '0')
        aportEnergetic++;

    energie += aportEnergetic * (1 - 0.02 * abs(MaxAge / 2 - varsta));      // formula pentru energie: aport * ( 1 - (mijlocul_varstei_maxime - varsta actuala) )
}

void Individ::giveBirth(Individ**& mat) {
    int FreeSpaces = 0, nrSons = 0;

    int x = i, y = j;
    while (++y < nrC && mat[x][y].isAlive() == '0')
        FreeSpaces++;
    y = j;
    while (--y > 0 && mat[x][y].isAlive() == '0')
        FreeSpaces++;

    x = i, y = j;

    while (++x < nrR && mat[x][y].isAlive() == '0')
        FreeSpaces++;
    x = i;
    while (--x > 0 && mat[x][y].isAlive() == '0')
        FreeSpaces++;

    nrSons = energie * (1 - 0.02 * abs(MaxAge / 2 - varsta));                      // formuala pentru numarul de fii: energie * (1 - |enegiaMax/2 - varsta| )
    nrSons = min(FreeSpaces, nrSons);                                       // numarul de fii nu poate fi mai mare decat numarul de spatii libere
    double neccesaryEnergyForGivingBirth = double(nrSons) * constGivingBirth;
    double MaxSonsRegardingEnergy = floor(energie / constGivingBirth);      //numarul maxim de fii pe care ii poate avea cu energia curenta
    if (energie < neccesaryEnergyForGivingBirth)                            //daca nrSons > nrMaxim la care poate da nastere atunci nrSons  =  nrMaxim la care poate da nastere                         
        nrSons = MaxSonsRegardingEnergy;
    energie -= nrSons * constGivingBirth;                                   // pentru fiecare fiu energia scade cu o consntanta
    x = i, y = j;
    while (++y < nrC && mat[x][y].isAlive() == '0' && nrSons--) {
        Individ aux(x, y, tip);
        mat[x][y] = aux;                                                    // un fiu de acelasi tip ia nastere(e apelat prin constructor)
    }

    y = j;
    while (--y > 0 && mat[x][y].isAlive() == '0' && nrSons--) {
        Individ aux(x, y, tip);
        mat[x][y] = aux;                                                    // un fiu de acelasi tip ia nastere(e apelat prin constructor)
    }

    x = i, y = j;
    while (++x < nrR && mat[x][y].isAlive() == '0' && nrSons--) {
        Individ aux(x, y, tip);
        mat[x][y] = aux;                                                    // un fiu de acelasi tip ia nastere(e apelat prin constructor)
    }

    x = i;
    while (--x > 0 && mat[x][y].isAlive() == '0' && nrSons--) {
        Individ aux(x, y, tip);
        mat[x][y] = aux;                                                    // un fiu de acelasi tip ia nastere(e apelat prin constructor)
    }
}

void Individ::attack(Individ**& mat) {
    int x = i, y = j;
    while (++y < nrC && int(mat[x][y].viu) != 48 && tip != mat[x][y].tip && energie > mat[x][y].energie) {
        energie -= mat[x][y].energie;
        mat[x][y].die();
    }
    x = i, y = j;
    while (--y > 0 && int(mat[x][y].viu) != 48 && tip != mat[x][y].tip && energie > mat[x][y].energie) {
        energie -= mat[x][y].energie;
        mat[x][y].die();
    }
                                                                            // ataca pe linie
    x = i, y = j;
    while (++x < nrR && int(mat[x][y].viu) != 48 && tip != mat[x][y].tip && energie > mat[x][y].energie) {
        energie -= mat[x][y].energie;
        mat[x][y].die();
    }
    x = i;
    while (--x > 0 && int(mat[x][y].viu) != 48 && tip != mat[x][y].tip && energie > mat[x][y].energie) {
        energie -= mat[x][y].energie;
        mat[x][y].die();
    }

                                                                            // ataca pe coloana
}

void Individ::getsOld() {
    varsta++;
    energie -= constGettingOld;
    if (energie <= 0 || varsta > MaxAge)
        die();
}

void Individ::die() {
    viu = 0;
    energie = 0;
    varsta = MaxAge + 1;
}

//metode publice

void Individ::update(Individ**& mat) {
    feed(mat);
    giveBirth(mat);
    attack(mat);
    getsOld();
}

unsigned char Individ::isAlive() {
    return viu;
}

char Individ::gettype() {
    return tip;
}

//metode tabla

void Individ::addOnTable(int x, int y, char type, Individ**& mat) {
    Individ aux(x, y, type);
    mat[x][y] = aux;
}

ostream& operator<<(std::ostream& os, Individ& mat) {
    if (mat.viu == '1')
        os << "{" << mat.tip << " " << mat.varsta << " " << mat.energie << " " << mat.viu << "}";
    else
        os << "{" << mat.tip << " " << mat.varsta << " " << mat.energie << " " << '0' << "}";

    return os;
}

void Individ::afisare(Individ**& mat) {
    cout << mat[i][j];
}

int main() {
    Individ** mat = new Individ * [nrR];
    for (int i = 0; i < nrR; ++i)                                            //matricea alocata dinamic
        mat[i] = new Individ[nrC];

    int i, j;
    i = 1; j = 1;

    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> distr(0, 6); // define the range

    for (i = 1; i < nrR; i++)
        for (j = 1; j < nrC; j++) {
            int choice = distr(gen);
            if (choice == 0)
                mat[i][j].addOnTable(i, j, '+', mat);                       //  adaug indivizi in matrice(sansa 5/7 sa las gol locul)
            else if (choice == 1)
                mat[i][j].addOnTable(i, j, '0', mat);
        }


    for (i = 1; i < nrR; i++) {
        for (j = 1; j < nrC; j++) {
            mat[i][j].afisare(mat);
            cout << " " << flush;
        }
        cout << endl;
    }
    cout << "===========================================CICLU" << endl << flush;
    i = 1; j = 1;

    bool KeepRunning = 1;
    int countAlive = 0;
    while (KeepRunning)
    {
        INPUT_RECORD Event = { 0 };
        DWORD r = 0;
        HANDLE consoleHandle = GetStdHandle(STD_INPUT_HANDLE);
        if (PeekConsoleInput(consoleHandle, &Event, 1, &r) && r > 0)
        {
            if (ReadConsoleInput(consoleHandle, &Event, 1, &r))
            {
                if (Event.EventType == KEY_EVENT)
                {
                    // It's a virtual key - You must use CAPITAL letters
                    if (Event.Event.KeyEvent.wVirtualKeyCode
                        && Event.Event.KeyEvent.bKeyDown != 0)                                      //cod preluat de aici: http://www.cplusplus.com/forum/beginner/108126/
                    {
                        // This happens when any key  is pressed, even if lowercase.
                        // The keys won't even be shown on the console,
                        // because the console will be in output mode.
                        KeepRunning = 0;
                    }
                }
            }
        }

        if (mat[i][j].isAlive() == '1') {
            countAlive++;
            mat[i][j].update(mat);
        }

        mat[i][j].afisare(mat);
        cout << " " << flush;
        if (j == nrC - 1) {
            cout << endl << flush;
            if (i == nrR - 1)
                cout << "===========================================CICLU" << endl << flush;
        }

        // cand ajunge pe poz nrR-nrC sa treaca in 1-1(nu 0-0, de aceea le incrementez)
        j = ((j + 1) % nrC);
        if (!j) {
            j++;
            i = ((i + 1) % nrR);
            if (!i) {
                i++;                                                    //incepe ciclu nou
                if (!countAlive)
                    KeepRunning = 0;                                     //insemana ca nu mai avem supravietuitori si se termina ciclul
                countAlive = 0;
            }
        }

    }

    for (int i = 0; i < nrR; ++i)
        delete[] mat[i];
    delete[] mat;                                                       //dealocarea memoriei
    return 0;
}

/*
Tema 16. (0.3 pct bonus) (TODO: indentare)

Program care genereaza aleator o tabela 20X70 de indivizi, majoritatea pozitiilor fiind insa goale, apoi intr-un ciclu, la fiecare iteratie apeleaza update, apoi afisaza tabla. Ciclarea dureaza pana se apasa o tasta sau pana nu mai ramane nici un individ pe tabla. Propunere: tabla sa fie ea insasi un obiect avand metode pentru initializare aleatoare, afisare, actualizare, test daca mai exista indivizi, alocarea unui nou element pe o pozitie data, eliminarea unui element de pe o pozitie data, etc.

*/
