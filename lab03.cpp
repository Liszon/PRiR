#include "lab03.h"
#include "kolory.h"

#include <thread>
#include <iostream>
#include <random>
using namespace std;


void wypelnijLosowymi(short T[], int n) {
    random_device maszynkaLosujaca;
    uniform_int_distribution<short> dystrybucja(-10,10);
    for(int i=0; i< n; i++) T[i] = dystrybucja(maszynkaLosujaca);
}


void wyswietlTablice(const short T[], int n){
    short suma = 0;

    for(int i=0; i<n; i++){
        cout << T[i] << " ";
        suma += T[i];
    }
    cout << endl << "Suma=" << suma << endl << endl;
}


void sumujOdDo(const short T[], int poczatek, int koniec, short & wynik){
    
    string k = Kolor::nastepny(), r = Kolor::reset(), info = "";
    
    wynik = 0;
    for(int i = poczatek; i< koniec; i++){
      
      info = k + to_string(T[i]) + r + "\n";
      cout << info;

      wynik += T[i];
    }

    info = k + "wynk=" + to_string(wynik) + "\n" + r;
    cout << info;
}

void sumujCoDrugi(const short T[], int poczatek, int koniec, short & wynik)
{
    string k = Kolor::nastepny(), r = Kolor::reset(), info = "";
    int i=poczatek;
    wynik = 0;
    while(i<koniec) 
    {
      
      info = k + to_string(T[i]) + r + "\n";
      cout << info;

      wynik += T[i];
      i=i+2;
    }

    info = k + "wynk=" + to_string(wynik) + "\n" + r;
    cout << info;
}

void sumujDodatnieLubUjemne(const short T[], bool warunek, int koniec, short & wynik)
{
    string k = Kolor::nastepny(), r = Kolor::reset(), info = "";
    int i=0;
    wynik = 0;
    if(warunek==true) //dodawanie dodatnich
    {
      while(i<koniec) 
      {
        info = k + to_string(T[i]) + r + "\n";
        cout << info;
        if(T[i]>0)
        wynik += T[i];
        i++;
      }
    }
    else
    {
      while(i<koniec) //dodawanie ujemnych
      {
        info = k + to_string(T[i]) + r + "\n";
        cout << info;
        if(T[i]<0)
        wynik += T[i];
        i++;
      }
    }

    info = k + "wynk=" + to_string(wynik) + "\n" + r;
    cout << info;
}

void doPotegi(short T[], int poczatek, int koniec)
{
    string k = Kolor::nastepny(), r = Kolor::reset(), info = "";
    
    for(int i = poczatek; i< koniec; i++){
      
      info = k + to_string(T[i]) + r + "\n";
      cout << info;

      T[i] = T[i]*T[i];
    }
}

void algorytm(short T[], short T2[], int poczatek, int koniec)
{
  string k = Kolor::nastepny(), r = Kolor::reset(), info = "";
    
    for(int i = poczatek; i< koniec; i++){
      
      info = k + to_string(T[i]) + r + "\n";
      cout << info;

      if(i==0)
      {
        T2[i]=(T[i]+T[i+1])/2;
      }
      else if(i==koniec)
      {
        T2[i]=(T[i]+T[i-1])/2;
      }
      else
      {
        T2[i]=(T[i-1]+T[i]+T[i+1])/3;
      }
    }
}


void zadanie_1(unsigned int rozmiarTablicy){
  
  cout << endl << "Zadanie 1" << endl << endl;

  static short * TAB; //Tablica
  TAB = new short[rozmiarTablicy];
  wypelnijLosowymi(TAB,rozmiarTablicy);
  wyswietlTablice(TAB,rozmiarTablicy);

  short wynik1, wynik2;
  unsigned int polowa = rozmiarTablicy /2;

  thread watek1(sumujOdDo,TAB,0,polowa,ref(wynik1));
  thread watek2(sumujOdDo,TAB,polowa,rozmiarTablicy,ref(wynik2));

  watek1.join();
  watek2.join();

  cout<< endl << "Wyniki pracy wątków" << endl
      << "suma1=" << wynik1 << " suma2=" << wynik2
      << " suma=" << wynik1 + wynik2 << endl << endl;

    delete [] TAB;
  
}

void zadanie_2(unsigned int rozmiarTablicy){
  
  cout << endl << "Zadanie 2" << endl << endl;

  static short * TAB; //Tablica
  TAB = new short[rozmiarTablicy];
  wypelnijLosowymi(TAB,rozmiarTablicy);
  wyswietlTablice(TAB,rozmiarTablicy);

  short wynik1, wynik2;

  thread watek1(sumujCoDrugi,TAB,0,rozmiarTablicy,ref(wynik1));
  thread watek2(sumujCoDrugi,TAB,1,rozmiarTablicy,ref(wynik2));

  watek1.join();
  watek2.join();

  cout<< endl << "Wyniki pracy wątków" << endl
      << "suma1=" << wynik1 << " suma2=" << wynik2
      << " suma=" << wynik1 + wynik2 << endl << endl;

    delete [] TAB;
  
}

void zadanie_3(unsigned int rozmiarTablicy){
  
  cout << endl << "Zadanie 3" << endl << endl;

  static short * TAB; //Tablica
  TAB = new short[rozmiarTablicy];
  wypelnijLosowymi(TAB,rozmiarTablicy);
  wyswietlTablice(TAB,rozmiarTablicy);

  short wynik1, wynik2;

  thread watek1(sumujDodatnieLubUjemne,TAB,true,rozmiarTablicy,ref(wynik1));
  thread watek2(sumujDodatnieLubUjemne,TAB,false,rozmiarTablicy,ref(wynik2));

  watek1.join();
  watek2.join();

  cout<< endl << "Wyniki pracy wątków" << endl
      << "suma1=" << wynik1 << " suma2=" << wynik2
      << " suma=" << wynik1 + wynik2 << endl << endl;

    delete [] TAB;
  
}

void zadanie_4(unsigned int rozmiarTablicy){
  
  cout << endl << "Zadanie 4" << endl << endl;

  static short * TAB; //Tablica
  TAB = new short[rozmiarTablicy];
  wypelnijLosowymi(TAB,rozmiarTablicy);
  wyswietlTablice(TAB,rozmiarTablicy);

  unsigned int polowa = rozmiarTablicy /2;

  thread watek1(doPotegi,TAB,0,polowa);
  thread watek2(doPotegi,TAB,polowa,rozmiarTablicy);

  watek1.join();
  watek2.join();

  cout<< endl << "Wyniki pracy wątków" << endl;
      wyswietlTablice(TAB,rozmiarTablicy);

    delete [] TAB;
  
}

void zadanie_5(unsigned int rozmiarTablicy){
  
  cout << endl << "Zadanie 5" << endl << endl;

  static short * TAB; //Tablica
  TAB = new short[rozmiarTablicy];
  wypelnijLosowymi(TAB,rozmiarTablicy);
  wyswietlTablice(TAB,rozmiarTablicy);

  static short * TAB2; //Tablica
  TAB2 = new short[rozmiarTablicy];

  unsigned int polowa = rozmiarTablicy /2;

  thread watek1(algorytm,TAB,TAB2,0,polowa);
  thread watek2(algorytm,TAB,TAB2,polowa,rozmiarTablicy);

  watek1.join();
  watek2.join();

  cout<< endl << "Wyniki pracy wątków" << endl;
      wyswietlTablice(TAB2,rozmiarTablicy);

    delete [] TAB;
    delete [] TAB2;
  
}