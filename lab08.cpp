#include "lab08.h"
#include <thread>
#include <mutex>
#include <random>
#include <iostream>
#include <set>
#include <vector>
#include "semafor.h"

using namespace std;

const int POJEMNOSC_HANGARU = 5;
const int LICZBA_SAMOLOTOW = 10;

random_device rd8;
minstd_rand generator8(rd8());
uniform_int_distribution<int> zakres8(1, 3);

mutex mutexKonsoli;

class Hangar;
class Samolot;

class Lotnisko{
public:
    Lotnisko(Hangar &h) : pasWolny(true), hangar(h), semaforPasStartowy(1) {

    }

    void laduj(Samolot &s);
    void kolujDoHangaru(Samolot &s);
    void oposcHangar(Samolot &s);
    void startuj(Samolot &s);

private:
    bool pasWolny;
    Hangar &hangar;
    Semafor semaforPasStartowy;
};

class Samolot{
public:
    Samolot(string n, Lotnisko &l) : nazwa(n), lotnisko(l) {

    }

    string nazwa;

    void operator()(){
        this_thread::sleep_for(chrono::seconds(zakres8(generator8)));
        lotnisko.laduj(*this);
        //postój przy hali przylotów
        this_thread::sleep_for(chrono::seconds(zakres8(generator8)));
        lotnisko.kolujDoHangaru(*this);
        //postój w hangarze
        this_thread::sleep_for(chrono::seconds(zakres8(generator8)));
        lotnisko.oposcHangar(*this);
        //postój przy hali odlotów
        this_thread::sleep_for(chrono::seconds(zakres8(generator8)));
        lotnisko.startuj(*this);
    }

private:
    Lotnisko &lotnisko;
};

class Hangar{
public:
    Hangar(int n) : rozmiarHangaru(n), semaforHangar(n){
    }

    void parkuj(Samolot &s) {
        if (samoloty.size() < rozmiarHangaru)
        {
            samoloty.insert(s.nazwa);
            cout<<"Samolot "+s.nazwa+" wjezdza do hangaru"<<endl;
        } else {
            cout<<"Hangar pelny"<<endl;
        }
    }

    void oposc(Samolot &s) {
        if (samoloty.count(s.nazwa) == 1)
        {
            samoloty.erase(s.nazwa);
            semaforHangar.sygnalizuj();
            cout<<"Samolot "+s.nazwa+" opuszcza hangar"<<endl;
        } else {
                cout<<"W hangarze nie ma tego samolotu"<<endl;
        }
    }

private:
    unsigned rozmiarHangaru;
    set<string> samoloty;
    Semafor semaforHangar;
};

void Lotnisko::laduj(Samolot &s) {
    semaforPasStartowy.czekaj();
    if (pasWolny)
    {
        pasWolny = false;
        string info = "Samolot "+s.nazwa+" laduje";
        cout<<info<<endl;
        this_thread::sleep_for(chrono::seconds(zakres8(generator8)));
        info = "Samolot "+s.nazwa+" wyladowal";
        cout<<info<<endl;
        pasWolny= true;
        semaforPasStartowy.sygnalizuj();
    } else {
        cout<<"Zderzenie na pasie startowym"<<endl;
    }
}

void Lotnisko::kolujDoHangaru(Samolot &s) {
    hangar.parkuj(s);
}

void Lotnisko::oposcHangar(Samolot &s) {
    hangar.oposc(s);
}

void Lotnisko::startuj(Samolot &s) {
    semaforPasStartowy.czekaj();
    if (pasWolny)
    {
        pasWolny = false;
        string info = "Samolot "+s.nazwa+" startuje";
        cout<<info<<endl;
        this_thread::sleep_for(chrono::seconds(zakres8(generator8)));
        info = "Samolot "+s.nazwa+" wystartowal";
        cout<<info<<endl;
        pasWolny = true;
        semaforPasStartowy.sygnalizuj();
    } else {
        cout<<"Zderzenie na pasie startowym"<<endl;
    }
}

void lab08() {
    Hangar hangar(POJEMNOSC_HANGARU);
    Lotnisko lotnisko(hangar);

    vector<thread> watki;

    for (int i=0; i<LICZBA_SAMOLOTOW; ++i)
    {
        string napis = "Rownolegle Linie Lotnicze "+ to_string(i);
        watki.emplace_back(Samolot(napis, lotnisko));
    }

    for (thread &w: watki) w.join();
}