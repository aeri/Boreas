#ifndef MONITORLinda
#define MONITORLinda

#include <queue>
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "tuplas.hpp"

using namespace std;

class MonitorLinda {
public:

    MonitorLinda();


    //----------------- Destructor
    ~MonitorLinda();


    void RemoveNote(Tupla t);

    void PostNote(Tupla t);

    

private:
    bool *surtidor;
    mutex mtxMonitor; //FUNDAMENTAL: mutex usarán las funcs 
    condition_variable haytupla;

    queue<int> coladim1;
    queue<int> coladim2;
    queue<int> coladim3;
    queue<int> coladim4;
    queue<int> coladim5;
    queue<int> coladim6;  
};
#endif