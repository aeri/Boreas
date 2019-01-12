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

using namespace std;

const int INTENTOS = 100;

//-----------------------------------------------------
int main(int argc, char* argv[])
{
    // faltan argumentos en la invocación?
    if(argc < 2)
	{
	    cerr << "Invocar como:" << endl;
	    cerr << "   test <ipServer> <portServer> <numTuplas>" << endl;
        cerr << "      <ipServer>: Número de tuplas a lanzar" << endl;
        cerr << "      <portServer>: Número de tuplas a lanzar" << endl;
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

    int i = 0;

    while (1){
        auto init = std::chrono::system_clock::now(); 
        entrada =start+to_string(i)+stop;
        ttp.from_string(entrada);
        
        LindaDriver.PN(ttp);


		

        cout << "Tupla número: " << i << " insertada correctamente."  << endl;

        ++i;

        auto end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - init);
        tiempo_poblado += elapsed.count();
        cout << "Tiempo transcurrido: " << elapsed.count() << " segundos." << endl;
    }


    cout << "Insertadas " << argv[3] << endl;

	
    LindaDriver.STOP();
    return 0;
}
