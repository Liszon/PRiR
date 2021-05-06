cmake_minimum_required(VERSION 3.19)
project(PRiR)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(PRiR
        kolory.cpp
        kolory.h
        lab01.cpp
        lab01.h
        lab02.cpp
        lab02.h
        lab03.cpp
        lab03.h
        lab04.cpp
        lab04.h
        lab05.cpp
        lab05.h
        lab06.cpp
        lab06.h
        lab07.cpp
        lab07.h
        main.cpp
        semafor.cpp
        semafor.h)
