
#ifndef _MONITORLINDA_HPP_
#define _MONITORLINDA_HPP_

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include "tuplas.hpp"
#include "Socket.hpp"

using namespace std;

class MonitorLinda
{
   public:
    MonitorLinda(); //constructor

    //----------------- Destructor
    ~MonitorLinda();

    void PostNote(Tupla t); //Añade una tupla

    void RemoveNote(Tupla t, Tupla& r); //Busca una tupla, toma su valor y la borra

    void ReadNote(Tupla t, Tupla& r);//Busca una tupla y borra su valor

   private:
    struct bbdd{ //Tipo Matriz
        struct Nodo
            {
                string valor; //
                Nodo* sigComp; //Siguiente componente de la tupla leida (vertical)
                Nodo* sigTupla; //Siguiente tupla (horizontal)
            };
            Nodo* primero; //Primer elemento de la primera tupla
    };

    bbdd tupleSpace1;//Matriz tuplas dimension 1
    bbdd tupleSpace2;//Matriz tuplas dimension 2
    bbdd tupleSpace3;//Matriz tuplas dimension 3
    bbdd tupleSpace4;//Matriz tuplas dimension 4
    bbdd tupleSpace5;//Matriz tuplas dimension 5
    bbdd tupleSpace6;//Matriz tuplas dimension 6

    mutex mtxMonitor;  // FUNDAMENTAL: mutex usarán las funcs

    condition_variable hay_tupla1; //indica si hay tuplas de dimension 1
    condition_variable hay_tupla2; //indica si hay tuplas de dimension 2
    condition_variable hay_tupla3; //indica si hay tuplas de dimension 3
    condition_variable hay_tupla4; //indica si hay tuplas de dimension 4
    condition_variable hay_tupla5; //indica si hay tuplas de dimension 5
    condition_variable hay_tupla6; //indica si hay tuplas de dimension 6
};
#endif
