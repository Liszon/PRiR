#include "lab05.h"
#include "kolory.h"
#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include <vector>
#include <random>
#include <condition_variable>

using namespace std;

//czas generowania znakow przez producenta
const int CZAS_PRACY = 5;

//Monitor - pojemnik przechowujący dane i zapewniający synchronizacje wątków
template <typename TypDanych>
class Monitor
{
  private:
    TypDanych dane;
    bool saDane;  //czy dane są aktualne

    mutex mutexDane;
    condition_variable warunekSaNoweDane; //umożliwia czekanie na nowe dane
    condition_variable warunekZabranoDane;  //umożliwia opóźnienie pojemnika
  public:
    Monitor()
    {
      saDane=false; //na start pusty pojemnik
    }

    void zapisz(TypDanych noweDane)
    {
      /*
      blokada umożliwia jednoczesny odczyt danych
      konieczne użycie unique_lock ponieważ zmienna warunku będzie musiała zdjąć blokade
      */
      unique_lock<mutex> blokada(mutexDane);
      //jeżeli pojemnik pełny czekamy na zebranie danych
      if(saDane) warunekZabranoDane.wait(blokada);
      saDane=true;
      dane=noweDane;
      //jeżeli na dane czeka inny wątek powiadamiamy go
      warunekSaNoweDane.notify_one();
      //blokada przestaje istnieć
    }

    TypDanych odczytaj()
    {
      //blokada umożliwia jednoczesny zapis danych
      unique_lock<mutex> blokada(mutexDane);
      //jeżeli brak danych to czekaj
      if(!saDane) warunekSaNoweDane.wait(blokada);
      saDane=false;
      warunekZabranoDane.notify_one();
      return dane;
    }
};

class Producent
{
  private:
    int czas; //czas pracy producenta
    Monitor< char >& pojemnik;  //referencja do wspólnego pojemnioka na dane
    char biezacyZnak;

    //generator liczb losowych
    minstd_rand generator;
    uniform_int_distribution< char > rozklad; //rozkład liczb pseudolosowych
  public:
    //tworzenie producenta generującego znaki [a,z]
    Producent(Monitor< char >& _pojemnik): czas(0), pojemnik(_pojemnik),biezacyZnak('a'),generator(random_device()()),rozklad('a','b')
    {
    }
    //kod wykonywany w wątku
    void operator()()
    {
      while(czas<CZAS_PRACY)
      {
        //generujemy kolejne znaki
        pojemnik.zapisz(biezacyZnak);
        //kolejny znak
        biezacyZnak=(biezacyZnak+1-97)%25+97;
        //przerwa w pracy
        this_thread::sleep_for(chrono::seconds(1));
        ++czas;
      }
      pojemnik.zapisz(EOF);
    }
};

class Konsument
{
  private:
    Monitor< char >& pojemnik;
  public:
    Konsument(Monitor< char >& _pojemnik): pojemnik(_pojemnik) {}

    //kod wykonywany w watku
    void operator()()
    {
      char znak = pojemnik.odczytaj();
      while (znak!=EOF)
      {
        cout<<znak<<endl;
        znak=pojemnik.odczytaj();
      }
    }
};

void lab05_cwiczenie_3_1(){
  cout<<endl<<"Zadanie 1"<<endl;
  random_device r;
  minstd_rand generator;
  //Tworzenie wspólnego pojemnika
  Monitor<char> pojemnik;
  Konsument konsument(pojemnik);
  Producent producent(pojemnik);
  thread wk(konsument);
  thread wp(producent);

  wp.join();
  wk.join();
}

//zadanie 2--------------------------------------------------------

class ProducentZad2
{
  private:
    int numer;  //numer producenta
    int czas; //czas pracy producenta
    Monitor< char >& pojemnik;  //referencja do wspólnego pojemnioka na dane
    char biezacyZnak;

    //generator liczb losowych
    minstd_rand generator;
    uniform_int_distribution< char > rozklad; //rozkład liczb pseudolosowych
  public:
    //tworzenie producenta generującego znaki [a,z]
    ProducentZad2(int _numer,Monitor< char >& _pojemnik): numer( _numer), czas(0), pojemnik(_pojemnik),biezacyZnak('a'),generator(random_device()()), rozklad('a','z')
    {
    }
    //kod wykonywany w wątku
    void operator()()
    {
      while(czas<CZAS_PRACY)
      {
        //generujemy kolejne znaki
        pojemnik.zapisz(biezacyZnak);
        //kolejny znak
        biezacyZnak=(biezacyZnak+1-97)%25+97;
        cout<<"Producent numer: "<<numer<<" |Biezacy znak: "<<biezacyZnak<<endl;
        //przerwa w pracy
        this_thread::sleep_for(chrono::seconds(1));
        ++czas;
      }
      pojemnik.zapisz(EOF);
    }
};

class KonsumentZad2
{
  private:
    int numer;  //numer konsumenta
    Monitor< char >& pojemnik;
  public:
    KonsumentZad2(int _numer,Monitor <char>& _pojemnik):numer( _numer),  pojemnik(_pojemnik) {}

    //kod wykonywany w watku
    void operator()()
    {
      char znak = pojemnik.odczytaj();
      while (znak!=EOF)
      {
        cout<<"Konsument numer: "<<numer<<" |Znak: "<<znak<<endl;
        znak=pojemnik.odczytaj();
      }
    }
};

void lab05_cwiczenie_3_2(){
  cout<<endl<<"Zadanie 2"<<endl;
  random_device r;
  minstd_rand generator;
  //Tworzenie wspólnego pojemnika
  Monitor<char> pojemnik;
  KonsumentZad2 konsumentZad2(1,pojemnik);
  ProducentZad2 producentZad2(1,pojemnik);
  KonsumentZad2 konsumentZad2_2(2,pojemnik);
  ProducentZad2 producentZad2_2(2,pojemnik);
  thread wk(konsumentZad2);
  thread wk2(konsumentZad2_2);
  thread wp(producentZad2);
  thread wp2(producentZad2_2);

  wp.join();
  wk.join();
  wp2.join();
  wk2.join();
}

//zadanie 3 -------------------------------------------------------------
template <typename TypDanych>
class MonitorZad3
{
  private:
    TypDanych dane;
    bool saDane;  //czy dane są aktualne

    mutex mutexDane;
    condition_variable warunekSaNoweDane; //umożliwia czekanie na nowe dane
    condition_variable warunekZabranoDane;  //umożliwia opóźnienie pojemnika
  public:
    MonitorZad3()
    {
      saDane=false; //na start pusty pojemnik
    }

    void zapisz(TypDanych noweDane)
    {
      // blokada umożliwia jednoczesny odczyt danych konieczne użycie unique_lock ponieważ zmienna warunku będzie musiała zdjąć blokade
      unique_lock<mutex> blokada(mutexDane);
      //jeżeli pojemnik pełny czekamy na zebranie danych
      if(saDane) warunekZabranoDane.wait(blokada);
      saDane=true;
      dane=noweDane;
      //jeżeli na dane czeka inny wątek powiadamiamy go
      warunekSaNoweDane.notify_one();
      //blokada przestaje istnieć
    }

    TypDanych odczytaj()
    {
      //blokada umożliwia jednoczesny zapis danych
      unique_lock<mutex> blokada(mutexDane);
      //jeżeli brak danych to czekaj
      if(!saDane) warunekSaNoweDane.wait(blokada);
      saDane=false;
      warunekZabranoDane.notify_one();
      return dane;
    }
};