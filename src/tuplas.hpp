#ifndef _TUPLAS_HPP_
#define _TUPLAS_HPP_

#include <cstring>
#include <iostream>

using namespace std;

class Tupla
{
   private:
    int dimension;
    string vector[6];

   public:
    Tupla(string p1);
    Tupla(string p1, string p2);
    Tupla(string p1, string p2, string p3);
    Tupla(string p1, string p2, string p3, string p4);
    Tupla(string p1, string p2, string p3, string p4, string p5);
    Tupla(string p1, string p2, string p3, string p4, string p5, string p6);
    Tupla(int dimension);
    void from_string(string tupla);
    string to_string();
    string get(int indice);
    void set(int indice, string campo);
    int size();
};

#endif