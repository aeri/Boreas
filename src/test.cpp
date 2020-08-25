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
#include <cstdlib>     /* srand, rand */
#include <ctime>
#include <string>
#include <chrono>

#include <unistd.h>

using namespace std;

const int INTENTOS = 100;

//-----------------------------------------------------
int main(int argc, char* argv[])
{
    // faltan argumentos en la invocación?
    if(argc < 3)
	{
	    cerr << "Invocar como:" << endl;
	    cerr << "   test <ipServer> <portServer> <numTuplas>" << endl;
        cerr << "      <ipServer>: IP del ServidorLinda" << endl;
        cerr << "      <portServer>: Puerto del ServidorLinda" << endl;
	    cerr << "      <numTuplas>: Número de tuplas a lanzar" << endl;
	    return 1;
	}

    srand (time(NULL));

    // un driver con los datos pasados en la invocación
    LD LindaDriver(argv[1], argv[2]);
    // La conexión con el servidor Linda ya está establecida

    cout << "Conexión establecida con éxito" << endl;

    //int dimension = rand() % 6 + 1;
    
    string entrada;
    Tupla ttp(6);
    string start="[a,b,c,d,e,";
    string stop="]";

    double tiempo_PN = 0.0;
    double tiempo_RN = 0.0;
    double tiempo_ReadN = 0.0;
    double tiempo_poblado = 0.0;

    string purria;

    for (int i = 0; i < atoi(argv[3]); ++i){
        
        entrada =start+to_string(i)+stop;
        ttp.from_string(entrada);
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto init = std::chrono::system_clock::now(); 
        LindaDriver.PN(ttp);
        auto end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);
		cout << "Tupla " << i << " en " << elapsed.count() << endl;
		tiempo_poblado += elapsed.count();

        cout << "PN realizado de tupla: " << ttp.to_string() << endl;
    }

    for (int i = 0; i < INTENTOS; ++i){
    	Tupla tpr("uno", "dos", "tres", "cuatro", "cinco", "seis");
    	// Tiempo de prueba de PostNote
		auto init = std::chrono::system_clock::now(); 

		LindaDriver.PN(tpr);

		auto end = std::chrono::system_clock::now(); 
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);

    	tiempo_PN += elapsed.count();
    	cout << "tiempo_PN " << tiempo_PN << endl;


    	entrada =start+to_string(i)+stop;
    	ttp.from_string(entrada);

    	// Tiempo de prueba de ReadNote

    	init = std::chrono::system_clock::now(); 

		LindaDriver.RD(ttp);

		end = std::chrono::system_clock::now(); 
		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);

    	tiempo_ReadN += elapsed.count();
    	

    	// Tiempo de prueba de RN
    	init = std::chrono::system_clock::now(); 

    	cout << "tiempo_ReadN " << tiempo_ReadN << endl;

		LindaDriver.RN(ttp);

		end = std::chrono::system_clock::now(); 
		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);

    	tiempo_RN += elapsed.count();

    	
    	cout << "tiempo_RN " << tiempo_RN << endl;

  
    }

    cout << "Insertadas " << argv[3] << endl;
	

	cout << "tiempo_poblado END " << tiempo_poblado/atoi(argv[3]) << endl;
	cout << "tiempo_PN END " << tiempo_PN/INTENTOS << endl;
	cout << "tiempo_ReadN END " << tiempo_ReadN/INTENTOS << endl;
	cout << "tiempo_RN END " << tiempo_RN/INTENTOS << endl;
	
    LindaDriver.STOP();

    pause();

    return 0;
}
