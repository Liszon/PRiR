#include "lab02.h"
#include "kolory.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

using namespace std;

//listing1_1
void kodWatku()
{
  string k = Kolor::nastepny(), r = Kolor::reset(); //opcjonalne kolorowanie dla lepszej czytelnosci

  for(int i=0; i<10; ++i)
  {
    cout<<k<<"Watek: "<<this_thread::get_id()<<" mowi czesc, i="<<i<<endl;
    this_thread::sleep_for(chrono::milliseconds(100));  //wstrzyanie watku na 100 milisekund
  }
}
  void listing1_1() //kod do wywolania w main
  {
    thread w1(kodWatku);
    thread w2(kodWatku);
    w1.join();
    w2.join();
  }

//listing1_2

const int LICZBA_PUNKTOW = 10;
double a = 0.0;
double b = M_PI;
double wynik = 0.0;

void calkaSinusa() 
{
  string k = Kolor::nastepny(), r = Kolor::reset(); //opcjonalne kolorowanie dla lepszej czytelnosci

  cout << k << "Watek pracuje " << r << endl;

  double h = (b - a ) / LICZBA_PUNKTOW;
  double s = sin(a);

  for(int i=1; i < LICZBA_PUNKTOW; i++){
    this_thread::sleep_for(chrono::milliseconds(10)); //wstrzyanie watku na 10 milisekund
    s += 2 * sin(a + i * h);
  }
  s += sin(b);

  wynik = s * h / 2;

  cout << k << "Watek się zakonczyl " << r << endl;

}

void listing1_2() //kod do wywolania w main
{

  thread watek(calkaSinusa);

  cout << "Sprzetowa wspolbieznosc: " <<thread::hardware_concurrency()<<endl;
  cout << "Id watku " << watek.get_id() << endl;

  watek.join();

  cout << "wynik= " << wynik << endl;
}

//listing1_3

class CalkaSinus
{
private:
    double a,b;
    double *wartosc;
public:
    CalkaSinus(double _a,double _b,double *_wartosc):a(_a), b(_b), wartosc(_wartosc){}

    void operator()();
};
void CalkaSinus::operator()()
{
    cout<<"Watek pracuje "<<endl;
    double h = (b-a)/LICZBA_PUNKTOW;
    double s = sin(a);
    for (int i = 1; i < LICZBA_PUNKTOW; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(10)); //wstrzyanie watku na 10 milisekund
        s += 2*sin(a+i*h);
    }
    s += sin(b);
    
    *wartosc = s*h/2;
    cout<<"Watek sie zakonczyl "<<endl;
}

void listing1_3()
{
    double wynik=0.0;
    CalkaSinus calka(0,M_PI,&wynik);  //tworzenie obiektu 
    thread watek(calka);  //tworzenie watku
    cout<<"Sprzetowa wspolbieznosc: "<<thread::hardware_concurrency()<<endl;
    cout<<"ID warku: "<<watek.get_id()<<endl;
    watek.join(); //oczekiwanie na zakonczenie watku
    cout<<"Wynik= "<<wynik<<endl;
}
