#include "lab04.h"
#include "kolory.h"

#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include <vector>
#include <random>

using namespace std;

//-----------------------------------------------------------------------------------

const int OPOZNIENIE = 100;
int licznik = 50;

class WatekA {
private:
  int numer;
  string k = Kolor::nastepny(), r = Kolor::reset() , info = "";

public:
  WatekA(int _numer): numer(_numer) {}
  void operator()(){
    while(licznik > 0){
      info = k + "w:" + to_string(numer) + " l:" + to_string(licznik);
      cout << info << endl;
      --licznik;
      //this_thread::sleep_for(chrono::microseconds(OPOZNIENIE));
      this_thread::sleep_for(chrono::microseconds(rand() % 500));

    }
  }
};

void lab04_cwiczenie_2_1(){
  cout<<endl<<"Zadanie 1"<<endl;
  srand (time(NULL));
  thread w1(WatekA(1));
  thread w2(WatekA(2));
  w1.join();
  w2.join();
}

//-----------------------------------------------------------------------------------

mutex licznikMutex;

class WatekB {
private:
  int numer;
  string k = Kolor::nastepny(), r = Kolor::reset() , info = "";

public:
  WatekB(int _numer): numer(_numer) {}
  void operator()(){
    while(true){
      {
        lock_guard<mutex> blokada(licznikMutex) ;
        if(licznik<=0) break;
        info = k + "w:" + to_string(numer) + " l:" + to_string(licznik);
        cout << info << endl;
        --licznik;
      }
      //this_thread::sleep_for(chrono::microseconds(OPOZNIENIE));
      this_thread::sleep_for(chrono::microseconds(rand() % 100));
    }
  }
};

void lab04_cwiczenie_2_2(){
  cout<<endl<<"Zadanie 2"<<endl;
  srand (time(NULL));
  thread w1(WatekB(1));
  thread w2(WatekB(2));
  w1.join();
  w2.join();
}

//-----------------------------------------------------------------------------------

mutex licznikMutexC;

class WatekC {
private:
  int numer;
  string k = Kolor::nastepny(), r = Kolor::reset() , info = "";

public:
  WatekC(int _numer): numer(_numer) {}
  void operator()(){
    while(true){
      
        licznikMutexC.lock();
        if(licznik<=0) break;
        info = k + "w:" + to_string(numer) + " l:" + to_string(licznik);
        cout << info << endl;
        --licznik;
        licznikMutexC.unlock();
      
      //this_thread::sleep_for(chrono::microseconds(OPOZNIENIE));
      this_thread::sleep_for(chrono::microseconds(rand() % 100));
    }
  }
};

void lab04_cwiczenie_2_3(){
  cout<<endl<<"Zadanie 3"<<endl;
  srand (time(NULL));
  thread w1(WatekC(1));
  thread w2(WatekC(2));
  w1.join();
  w2.join();
}

//-----------------------------------------------------------------------------------

const int LICZBA_WATKOW = 5;
const int ZNAKOW_DO_ZAPISANIA = 1024;

mutex licznikMutexD;

class WatekD {
private:
  char znak;
  string &lancuch;
  string k = Kolor::nastepny(), r = Kolor::reset() , info = "";

public:
  WatekD(char _znak, string &_lancuch): znak(_znak), lancuch(_lancuch) {}
  void operator()(){
    {
      lock_guard<mutex> blokada(licznikMutexD);
      for(int i = 0;i<ZNAKOW_DO_ZAPISANIA;++i) {
        this_thread::sleep_for(chrono::microseconds(OPOZNIENIE));
        
        lancuch[i] = znak;
      }
      info = k+"w:" + znak + "l:" + lancuch;
      cout<< info << endl;
    }
    
  }
};

void lab04_cwiczenie_2_4(){
  cout<<endl<<"Zadanie 4"<<endl;
  srand (time(NULL));
  string lancuch;
  lancuch.resize(ZNAKOW_DO_ZAPISANIA);
  vector<thread> watki;
  for(unsigned int i = 1;i<LICZBA_WATKOW; ++i){
    watki.push_back(thread(WatekD('a'+i,lancuch)));
    //this_thread::sleep_for(chrono::microseconds(OPOZNIENIE));
  }
  for(thread& w: watki) w.join();
  cout << lancuch<< endl;
}

//-----------------------------------------------------------------------------------

unsigned int * TAB;
mutex licznikMutexDodatkowe;

void wypelnijLosowymi2(unsigned int T[]) {
  unsigned int suma=0;
  unsigned int wylosowana;
  
    random_device maszynkaLosujaca;
    uniform_int_distribution<unsigned int> dystrybucja(10,200);
    for(int i=0; i< 20; i++) {
      wylosowana = dystrybucja(maszynkaLosujaca);
      T[i] = wylosowana;
      suma+=wylosowana;
    }
    cout << endl << "Suma=" << suma << endl << endl;
    
}
unsigned int wylosujLiczbe() {
    random_device maszynkaLosujaca;
    uniform_int_distribution<unsigned int> dystrybucja(0,19);
    return dystrybucja(maszynkaLosujaca);
}
bool czyWyzerowana(unsigned int *t) {
  int licz = 0;
  for(int i =0; i<20;i++) {
    if(t[i] == 0){
      licz++;
    }
  }
  if(licz ==20) return true;
  else return false;
}


class WatekDodatkowe {
private:
  int numer;
  int licznikWew = 0;
  string k = Kolor::nastepny(), r = Kolor::reset() , info = "";

public:
  WatekDodatkowe(int _numer): numer(_numer) {}
  void operator()(){
    while(true){
      {
        lock_guard<mutex> blokada(licznikMutexDodatkowe);
        unsigned int wylosowaneMiejsce = wylosujLiczbe();
        if(TAB[wylosowaneMiejsce]!=0) {
          info = k+"w: Wylosowałem: "+to_string(wylosowaneMiejsce) +  " > Napotkalem: " + to_string(TAB[wylosowaneMiejsce])+" > *ZERUJE* | LICZNIK = "+ to_string(licznikWew);
          cout<< info << endl;
          licznikWew += TAB[wylosowaneMiejsce];
          TAB[wylosowaneMiejsce]=0;
        } else {
          info = k+"w: Wylosowałem: "+to_string(wylosowaneMiejsce) +  " > Napotkalem: " + to_string(TAB[wylosowaneMiejsce]) +" > NIE ZERUJE | LICZNIK = "+ to_string(licznikWew);
          cout<< info << endl;
        }

        if(czyWyzerowana(TAB)) {
          info = k+"w:" + to_string(numer) + "Suma: " + to_string(licznikWew);
          cout<< info << endl;
          break;
        }
      }
      this_thread::sleep_for(chrono::microseconds(OPOZNIENIE+ licznikWew*100));
      
      
      //this_thread::sleep_for(chrono::microseconds(rand() % 100));
    }
  }
};
void lab04_zadanie_dodatkowe(){
  cout<<endl<<"Zadanie dodatkowe"<<endl;
  TAB = new unsigned int[20];
  wypelnijLosowymi2(TAB);
  thread w1(WatekDodatkowe(1));
  thread w2(WatekDodatkowe(2));
  w1.join();
  w2.join();
  

}