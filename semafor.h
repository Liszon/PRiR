#ifndef SEMAFOR_H_
#define SEMAFOR_H_

#include <mutex>
#include <condition_variable>

using namespace std;

//służy do zarzadzania dostępem do ograniczonych zasobów

class Semafor{
public:
    Semafor(int _count=1);
    
    void sygnalizuj();  //zwalnia zasób
    void czekaj();  //zajmuje zasób (jeżeli nie ma wolnych to czeka na zwolnienie zasobu)
    bool proboj_czekac();   //proboje zająć zasob, true - udało się | false - wszystkie zasoby zajęte
    int wartosc() const;    //zwraca bierzącą wartośc licznika
    
    Semafor(const Semafor &obj);

private:
    int licznikZasobow; //licznik dostępnych zasobów
    //synchronizacja licznika
    mutex mutexLicznika;    
    condition_variable warunekLicznikRoznyOdZera;
};

#endif