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
    MonitorLinda();

    //----------------- Destructor
    ~MonitorLinda();

    void PostNote(Tupla t);

    void RemoveNote(Tupla t, Tupla& r);

    void ReadNote(Tupla t, Tupla& r);

   private:
    struct bbdd{
        struct Nodo
            {
                string valor;
                Nodo* sigComp;
                Nodo* sigTupla;
            };
            Nodo* primero;
    };

    bbdd tupleSpace1;
    bbdd tupleSpace2;
    bbdd tupleSpace3;
    bbdd tupleSpace4;
    bbdd tupleSpace5;
    bbdd tupleSpace6;

    mutex mtxMonitor;  // FUNDAMENTAL: mutex usarán las funcs

    condition_variable hay_tupla1;
    condition_variable hay_tupla2;
    condition_variable hay_tupla3;
    condition_variable hay_tupla4;
    condition_variable hay_tupla5;
    condition_variable hay_tupla6;
};
#endif