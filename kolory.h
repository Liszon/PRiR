#ifndef KOLORY_H
#define KOLORY_H

#include <string>
#include <mutex>

class Kolor{
private:
    static unsigned short licznik;
    static std::mutex mtx;
    static const std::string RESET;
    static const std::string KOLOR[];
public:
    static const unsigned short LICZBA_KOLOROW = 12;
    static std::string reset();
    static std::string nastepny();
    static std::string test();
};

#endif 