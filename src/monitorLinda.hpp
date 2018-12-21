#ifndef _MONITORLINDA_HPP_
#define _MONITORLINDA_HPP_

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include "tuplas.hpp"

using namespace std;

class MonitorLinda
{
   public:
    MonitorLinda(const string ip1,
                    const int p1,
                    const string ip2,
                    const int p2,
                    const string ip3,
                    const int p3);

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
    condition_variable haytupla4;
    condition_variable haytupla5;
    condition_variable haytupla6;

    void MonitorLinda::PostNote(Tupla t);

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