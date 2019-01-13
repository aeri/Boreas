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
	    cerr << "   test <ipServer> <portServer>" << endl;
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
    string entradb;
    string entradc;
    Tupla tta(6);
    Tupla ttb(4);
    Tupla ttc(2);
    string starta="[a,b,c,d,e,";
    string startb="[a,b,c,";
    string startc="[a,";
    string stop="]";






    double tiempo_PN = 0.0;
    double tiempo_RN = 0.0;
    double tiempo_ReadN = 0.0;

    int j = 0;

    while (j < 1000){
       
        entrada =starta+to_string(j)+stop;
        entradb =startb+to_string(j)+stop;
        entradc =startc+to_string(j)+stop;
        tta.from_string(entrada);
        ttb.from_string(entradb);
        ttc.from_string(entradc);
        
        LindaDriver.PN(tta);
        LindaDriver.PN(ttb);
        LindaDriver.PN(ttc);

        cout << "Tupla número: " << j << " insertada correctamente."  << endl;

        ++j;
    }

    for (int i = 0; i < INTENTOS; ++i){
        Tupla tp6("uno", "dos", "tres", "cuatro", "cinco", "seis");

        // Tiempo de prueba de PostNote
        auto init = std::chrono::system_clock::now(); 

        LindaDriver.PN(tp6);

        auto end = std::chrono::system_clock::now(); 
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);

        tiempo_PN += elapsed.count();
        cout << "tiempo_PN " << tiempo_PN << endl;

        entrada =starta+to_string(i)+stop;
        tta.from_string(entrada);


        // Tiempo de prueba de ReadNote

        init = std::chrono::system_clock::now(); 

        LindaDriver.ReadN(tta);

        end = std::chrono::system_clock::now(); 
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);

        tiempo_ReadN += elapsed.count();
        

        // Tiempo de prueba de RN
        init = std::chrono::system_clock::now(); 

        cout << "tiempo_ReadN " << tiempo_ReadN << endl;

        LindaDriver.RN(tta);

        end = std::chrono::system_clock::now(); 
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);

        tiempo_RN += elapsed.count();

        
        cout << "tiempo_RN " << tiempo_RN << endl;

  
    }

   

    cout << "tiempo_PN END " << tiempo_PN/INTENTOS << endl;
    cout << "tiempo_ReadN END " << tiempo_ReadN/INTENTOS << endl;
    cout << "tiempo_RN END " << tiempo_RN/INTENTOS << endl;
    
    LindaDriver.STOP();
    return 0;
}
