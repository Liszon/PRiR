#include "lab07.h"
#include "iostream"
#include <string>
#include <random>
#include <thread>
#include <vector>
#include "semafor.h"

using namespace std;
const int MAX_OPOZNIENIE = 1000;
const int LICZBA_FILOZOFOW = 5;
const int LICZBA_POSILKOW = 3;

random_device rd;
minstd_rand generator2(rd());
uniform_int_distribution<int> rozklad(0, MAX_OPOZNIENIE);

class Widelec {
private:
    int numer;
    Semafor semafor;
public:
    Widelec(int n) : numer(n), semafor(1) {

    }

    void czekaj() {
        semafor.czekaj();
    }

    void sygnalizuj() {
        semafor.sygnalizuj();
    }

};

class Filozof {
private:
    int numer;
    int lewyWidelec;
    int prawyWidelec;
    int zjedzonychPosilkow;
    vector<Widelec> &widelce;
    Semafor &iloscJedzacychSemafor;
public:
Filozof (int n, vector<Widelec> &_widelce, Semafor &_iloscJedzacychSemafor) : numer(n), zjedzonychPosilkow(0), widelce(_widelce), iloscJedzacychSemafor(_iloscJedzacychSemafor)
{
  lewyWidelec = numer;
  prawyWidelec = (numer - 1);
  if (prawyWidelec == -1)
  {
    prawyWidelec = LICZBA_FILOZOFOW - 1;
  }
}

void operator()()
{
  while (zjedzonychPosilkow < LICZBA_POSILKOW)
  {
    iloscJedzacychSemafor.czekaj();
     widelce[lewyWidelec].czekaj();

    cout << "Filozof " + std::to_string(numer) + " podnosi widelec: " + std::to_string(lewyWidelec) + " \n";

    widelce[prawyWidelec].czekaj();
    cout << "Filozof " + std::to_string(numer) + " podnosi widelec: " + std::to_string(prawyWidelec) + " \n";

    int czasPosilku = rozklad(generator2);
    zjedzonychPosilkow++;

    cout << "Filozof " + std::to_string(numer) + " je widelcami: " + std::to_string(lewyWidelec) + " i " + std::to_string(prawyWidelec) + " posilek: " + std::to_string(zjedzonychPosilkow) + " przez najblizsze " + std::to_string(czasPosilku) + " ms \n";

    this_thread::sleep_for(chrono::milliseconds(czasPosilku));

    cout << "Filozof " + std::to_string(numer) + " skonczyl jesc " + std::to_string(zjedzonychPosilkow) + " posilek " + " \n";

    widelce[lewyWidelec].sygnalizuj();
    widelce[prawyWidelec].sygnalizuj();
    iloscJedzacychSemafor.sygnalizuj();
  }
    cout << "Filozof " + std::to_string(numer) + " sie najadl \n ";
}
};

void lab07()
{
  vector<Widelec> widelce;

    for (int i = 0; i < LICZBA_FILOZOFOW; ++i) {
        Widelec w(i);
        widelce.push_back(w);
    }

    vector<thread> filozofowie;
    Semafor iloscJedzacychSemafor(4);

    for (int i = 0; i < LICZBA_FILOZOFOW; ++i) {
        filozofowie.emplace_back(Filozof(i, widelce, iloscJedzacychSemafor));
    }

    for (thread &f: filozofowie) {
        f.join();
    }
}