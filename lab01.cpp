#include "kolory.h"
#include <iostream>
#include <thread>
#include <chrono>

void kodDoWykonania() {

  std::thread::id id = std::this_thread::get_id();
  std::string kolor = Kolor::nastepny();
  
  for(int i=0; i<10; i++){
    
    std::cout << kolor << "Watek id=" << id;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    std::cout << " odlicza: " << i;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    std::cout << Kolor::reset() << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void wykonajSekwencyjne(){

  kodDoWykonania();
  kodDoWykonania();
  kodDoWykonania();
  kodDoWykonania();
  kodDoWykonania();
  
}

void wykonajRownolegle(){
  
  std::thread watek1(kodDoWykonania);
  std::thread watek2(kodDoWykonania);
  std::thread watek3(kodDoWykonania);
  std::thread watek4(kodDoWykonania);
  std::thread watek5(kodDoWykonania);

  watek1.join();
  watek2.join();
  watek3.join();
  watek4.join();
  watek5.join();

}