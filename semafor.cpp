#include "semafor.h"

int Semafor::wartosc() const 
{
    return licznikZasobow;
}

void Semafor::sygnalizuj() 
{
    unique_lock<mutex> blokadaLicznika(mutexLicznika);  //licznik na wyłączność
    ++licznikZasobow;   //inkrementacja licznika
    warunekLicznikRoznyOdZera.notify_one(); //powiadamiamy jeden oczekujący wątek że licznik został zwiększony
}

void Semafor::czekaj() 
{
    unique_lock<mutex> blokadaLicznika(mutexLicznika);  //licznik na wyłącznośc
//jezeli licznik jest równy 0 to nie możemy go zmniejszyć, czekamy aż zostanie zwiększony przez inny wątek
    while (licznikZasobow == 0)
    {
        warunekLicznikRoznyOdZera.wait(blokadaLicznika);
    }
    --licznikZasobow; //dekrementacja licznika
}

bool Semafor::proboj_czekac() 
{
    unique_lock<mutex> blokadaLicznika(mutexLicznika);  //licznik na wyłączność
    
    if (licznikZasobow != 0)    //jeśli licznik większy od 0 to go zmniejszamy
    {
        --licznikZasobow;
        return true;
    } else
    {
        return false;
    }
}