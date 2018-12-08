#include <queue>
#include "monitorLinda.hpp"
#include <cassert>


//si DEBUG_MODE, generará eventos en log; si no, no hará nada
#if DEBUG_MODE 
    #define LOG(l,s) l->addMessage(s); //LOG(log, mensaje): añade "mensaje" evento en "log"
#else
    #define LOG(l,s)                   //LOG(log, mensaje): no hace nada
#endif


//-----------------------------------------------------
MonitorLinda::MonitorLinda(const int NS) {
	surtidor = new bool[NS];
	for (int i = 0; i < NS; i++) {
		surtidor[i] = false;
	}
	this->NS = NS;
	nLibres = NS;
	quiereMantenimiento = false;
};

MonitorLinda::MonitorLinda(const int NS, Logger *logger) {
	surtidor = new bool[NS];
	for (int i = 0; i < NS; i++) {
		surtidor[i] = false;
	}
	this->NS = NS;
	log = logger;  
	nLibres = NS;
	quiereMantenimiento = false;
};


MonitorLinda::~MonitorLinda() {
 	delete[] surtidor;
};

    MonitorLinda();


    //----------------- Destructor
    ~MonitorLinda();


    void RemoveNote(Tupla t) {
    	unique_lock<mutex> lck(mtxMonitor);

    	int dimension = t.size();
    	switch (dimension) {
    		case 1:
    			coladim1.push(t);
    		case 2:
    			coladim2.push(t);
    		case 3:
    			coladim3.push(t);
    		case 4:
    			coladim4.push(t);
    		case 5:
    			coladim5.push(t);
    		case 6:
    			coladim6.push(t);
    	}


    };

    void PostNote(Tupla t) {
    	unique_lock<mutex> lck(mtxMonitor);
    }






