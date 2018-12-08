#ifndef MONITOR
#define MONITOR

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>

using namespace std;

class ControlGasolinera {
public:
    //----------------- Constructores
    // <NS>: número de surtidores de la gasolinera
    ControlGasolinera(const int NS);

    // <NS>: número de surtidores de la gasolinera
    // <logger>: referencia a un objeto de la clase "Logger"
    ControlGasolinera(const int NS, Logger *logger);

    //----------------- Destructor
    ~ControlGasolinera();

    // Se bloquea hasta que hay un surtidor libre
    // Devuelve en el <surt> un numero de surtidor, que estaba libre. El surtidor pasa a estar ocupado
    void dameSurtidor(int &surt);

    // Libera el surtidor <surt>
    void dejoSurtidor(const int surt);

    // Avisa de que va a empezar un proceso de mantenimiento.
    // Desde este momento, ningún surtidor, aunque esté libre, será concedido
    // Cuando todos los surtidores estén libres, podrá llevar a cabo el mantenimiento
    void beginMantenimiento();

    // Avisa de fin de tarea de mantenimiento. A partir de este momento ya podrán
    // concederse surtidores
    void endMantenimiento();

private:
    int nLibres;  //num de surtidores libres
    int NS;
    bool *surtidor;
    bool quiereMantenimiento;
    Logger *log;
    mutex mtxMonitor; //FUNDAMENTAL: mutex usarán las funcs 
    condition_variable estaLibre;
    condition_variable estaMantenimiento;
};
#endif