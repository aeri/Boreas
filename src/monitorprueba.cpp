#include "ControlGasolinera.hpp"
#include <cassert>


//si DEBUG_MODE, generará eventos en log; si no, no hará nada
#if DEBUG_MODE 
    #define LOG(l,s) l->addMessage(s); //LOG(log, mensaje): añade "mensaje" evento en "log"
#else
    #define LOG(l,s)                   //LOG(log, mensaje): no hace nada
#endif


//-----------------------------------------------------
ControlGasolinera::ControlGasolinera(const int NS) {
	surtidor = new bool[NS];
	for (int i = 0; i < NS; i++) {
		surtidor[i] = false;
	}
	this->NS = NS;
	nLibres = NS;
	quiereMantenimiento = false;
};

ControlGasolinera::ControlGasolinera(const int NS, Logger *logger) {
	surtidor = new bool[NS];
	for (int i = 0; i < NS; i++) {
		surtidor[i] = false;
	}
	this->NS = NS;
	log = logger;  
	nLibres = NS;
	quiereMantenimiento = false;
};


ControlGasolinera::~ControlGasolinera() {
 	delete[] surtidor;
};

    // Se bloquea hasta que hay un surt libre
    // Devuelve en el <surt> un numero de surt, que estaba libre. El surt pasa a estar ocupado
void ControlGasolinera::dameSurtidor(int &surt) {
	unique_lock<mutex> lck(mtxMonitor);	
	LOG(log,"BEGIN_FUNC_PROC,dameSurtidor, " + to_string(nLibres));
	while ((nLibres <= 0) || quiereMantenimiento) { 
		estaLibre.wait(lck);
	}
	nLibres--;
    surt = 0;
    while (!surtidor[surt] && surt < NS) {
        surt++; 
    } 
	surtidor[surt-1] = true;
	LOG(log,"END_FUNC_PROC,dameSurtidor, "+to_string(nLibres));
};

// Libera el surt <surt>
void ControlGasolinera::dejoSurtidor(const int surt) {
	unique_lock<mutex> lck(mtxMonitor);
	LOG(log,"BEGIN_FUNC_PROC,dejoSurtidor, " + to_string(nLibres));
	nLibres++;
	surtidor[surt-1] = false;
	if (quiereMantenimiento && (nLibres == NS)) {
		estaMantenimiento.notify_one();
	}
	else {
		estaLibre.notify_all();
	}
	LOG(log,"END_FUNC_PROC,dejoSurtidor, " + to_string(nLibres));
};

// Avisa de que va a empezar un proceso de mantenimiento.
// Desde este momento, ningún surt, aunque esté libre, será concedido
// Cuando todos los surtes estén libres, podrá llevar a cabo el mantenimiento
void ControlGasolinera::beginMantenimiento() {
	unique_lock<mutex> lck(mtxMonitor);
	LOG(log,"BEGIN_FUNC_PROC,beginMantenimiento, " + to_string(nLibres));
	quiereMantenimiento = true;
	while (nLibres < NS) {
		estaMantenimiento.wait(lck);
	}
	LOG(log,"END_FUNC_PROC,beginMantenimiento, " + to_string(nLibres));
};

// Avisa de fin de tarea de mantenimiento. A partir de este momento ya podrán
// concederse surtes
void ControlGasolinera::endMantenimiento() {
	unique_lock<mutex> lck(mtxMonitor);
	LOG(log,"BEGIN_FUNC_PROC,endMantenimiento, " + to_string(nLibres));
	quiereMantenimiento = false;
	estaLibre.notify_all();	
	LOG(log,"END_FUNC_PROC,endMantenimiento, " + to_string(nLibres));
};
