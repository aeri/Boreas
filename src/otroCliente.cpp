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

using namespace std;

//-----------------------------------------------------
int main(int argc, char* argv[])
{
    // faltan argumentos en la invocación?
    if(argc < 2)
	{
	    cerr << "Invocar como:" << endl;
	    cerr << "   mainLindaDriver <IP_LS> <Port_LS>" << endl;
	    cerr << "      <IP_LS>: IP del servidor Linda" << endl;
	    cerr << "      <Port_LS>: puerto del servidor Linda" << endl;
	    return 1;
	}

    // un driver con los datos pasados en la invocación
    LD LindaDriver(argv[1], argv[2]);
    // La conexión con el servidor Linda ya está establecida

    cout << "Conexión establecida con éxito" << endl;
    // Varias formas de construir tuplas
    Tupla ttp("500", "mi casa", "árbol");
    LindaDriver.PN(ttp);
    cout << "El cliente recibe: " << ttp.to_string() << endl;
    while (1);
  

    return 0;
}