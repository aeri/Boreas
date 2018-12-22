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

    void RemoveNote(Tupla t);

    void ReadNote(Tupla t);

    void PostNote(Tupla t);

   private:
    mutex mtxMonitor;  // FUNDAMENTAL: mutex usarán las funcs

    condition_variable haytupla1;
    condition_variable haytupla2;
    condition_variable haytupla3;

    string ip1;
    int p1;
    string ip2;
    int p2;
    string ip3;
    int p3;


    /*
    queue<int> coladim1;
    queue<int> coladim2;
    queue<int> coladim3;
    queue<int> coladim4;
    queue<int> coladim5;
    queue<int> coladim6;
    */
};
#endif