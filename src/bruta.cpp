//*****************************************************************
// File:   mainLindaDriver.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2016
// Coms:   Esqueleto para prueba del TP-6
//         invocar mediante
//            mainLindaDriver IP_LindaServer Port_LindaServer ...
//*****************************************************************

#include "LindaDriver.hpp"
#include "tuplas.hpp"
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

const int INTENTOS = 100;

//-----------------------------------------------------
int main(int argc, char* argv[])
{
    // faltan argumentos en la invocación?
    if(argc < 2)
	{
	    cerr << "Invocar como:" << endl;
	    cerr << "   test <ipServer> <portServer>" << endl;
        cerr << "      <ipServer>: Número de tuplas a lanzar" << endl;
        cerr << "      <portServer>: Número de tuplas a lanzar" << endl;
	    return 1;
	}



    // un driver con los datos pasados en la invocación
    LD LindaDriver(argv[1], argv[2]);
    // La conexión con el servidor Linda ya está establecida

    cout << "Conexión establecida con éxito" << endl;

    //int dimension = rand() % 6 + 1;
    
    string entrada;
    Tupla ttp(6);
    string start="[a,b,c,d,e,";
    string stop="]";

    double tiempo_poblado = 0.0;

    int i = 0;

    while (1){
        auto init = std::chrono::system_clock::now(); 
        entrada =start+to_string(i)+stop;
        ttp.from_string(entrada);
        
        LindaDriver.PN(ttp);


		

        cout << "Tupla número: " << i << " insertada correctamente."  << endl;

        ++i;

        auto end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);
        tiempo_poblado += (elapsed.count() / 1000.0);
        cout << "Tiempo transcurrido: " << tiempo_poblado << " segundos." << endl;
    }
	
    LindaDriver.STOP();
    return 0;
}
