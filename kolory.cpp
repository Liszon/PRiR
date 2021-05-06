#include "kolory.h"

unsigned short Kolor::licznik = 0;
std::mutex Kolor::mtx;

const std::string Kolor::RESET = "\033[0m";

const std::string Kolor::KOLOR[] ={
    "\033[0;31m", //01
    "\033[1;32m", //02
    "\033[1;33m", //03
    "\033[1;34m", //04
    "\033[0;35m", //05
    "\033[0;43m", //06
    "\033[0;44m", //07
    "\033[0;45m", //08
    "\033[0;46m", //09
    "\033[0;40m", //10
    "\033[0;41m", //11
    "\033[0;47m", //12
};

std::string Kolor::reset() {
    return RESET;
}

std::string Kolor::nastepny(){
    mtx.lock();
    std::string kolor = KOLOR[licznik];
    licznik = ++licznik < LICZBA_KOLOROW ? licznik : 0;
    mtx.unlock();
    return kolor;
}

std::string Kolor::test(){
  std::string wynik = "\n---Test kolorów---\n";
  for(int i=1; i<= Kolor::LICZBA_KOLOROW; i++){
    wynik += Kolor::nastepny() + "Kolor " + std::to_string(i) + "\n";
  }
  wynik += Kolor::reset() + "---Koniec testu---\n";
return wynik;
}