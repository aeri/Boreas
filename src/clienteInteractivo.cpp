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

#include <chrono>

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
    cout << "Conexión al servidor " << argv[1] << " establecida..." << endl;

    bool stop = false;
    int operacion;
    int dimension;

    cout << "Cliente interactivo Version 0.1" << endl;
    cout << "1. PN" << endl;
    cout << "2. RM" << endl;
    cout << "3. ReadN" << endl;
    cout << "0. Parar" << endl;

    while(!stop)
	{
	    cout << endl << "Introduzca operación: " << flush;
	    cin >> operacion;

	    int i = 1;
	    string componente;

	    if(operacion != 0)
		{
		    cout << "Introduzca dimensión: " << flush;
		    cin >> dimension;
		    Tupla tta(dimension);
		    while(i <= dimension)
			{
			    cout << "Introduce la componente de la tupla " << i << ": " << flush;
			    cin >> componente;
			    tta.set(i, componente);
			    ++i;
			} 

		    if(operacion == 1)
			{
			    cout << "Se realiza PostNote de " << tta.to_string() << endl;
			    auto init = std::chrono::system_clock::now();
			    LindaDriver.PN(tta);
			    auto end = std::chrono::system_clock::now();
			    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);
				
				cout << "Operacion realizada en: " << elapsed.count() << " milisegundos." << endl;
			}
		    else if(operacion == 2)
			{
			    cout << "Se realiza RemoveNote de " << tta.to_string() << endl;
			    auto init = std::chrono::system_clock::now();
			    cout << "Se devuelve: " << LindaDriver.RN(tta).to_string() << endl;
			    auto end = std::chrono::system_clock::now();
			    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);
				
				cout << "Operacion realizada en: " << elapsed.count() << " milisegundos." << endl;
			}
		    else if(operacion == 3)
			{
			    cout << "Se realiza ReadNote de " << tta.to_string() << endl;
			    auto init = std::chrono::system_clock::now();
			    cout << "Se devuelve: " << LindaDriver.ReadN(tta).to_string() << endl;
			    auto end = std::chrono::system_clock::now();
			    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);
				
				cout << "Operacion realizada en: " << elapsed.count() << " milisegundos." << endl;
			}

			
			
		}
	    else
		{
		    cout << "Ejecución finalizada" << endl;
		    LindaDriver.STOP();
		    stop = true;
		}

	    /*
	            switch (dimension){
	                case 1:
	                    string a;
	                    cin >> a;
	                    tta.set(1,a);
	                    break;
	                case 2:
	                    string a,b;
	                    cin >> a >> b;
	                    tta.set(1,a);
	                    tta.set(2,b);
	                    break;
	                case 3:
	                    string a,b,c;
	                    cin >> a >> b >> c;
	                    tta.set(1,a);
	                    tta.set(2,b);
	                    tta.set(3,c);
	                    break;
	                case 4:
	                    string a,b,c,d;
	                    cin >> a >> b >> c >> d;
	                    tta.set(1,a);
	                    tta.set(2,b);
	                    tta.set(3,c);
	                    tta.set(4,d);
	                    break;
	                case 5:
	                    string a,b,c,d,e;
	                    cin >> a >> b >> c >> d >> e;
	                    tta.set(1,a);
	                    tta.set(2,b);
	                    tta.set(3,c);
	                    tta.set(4,d);
	                    tta.set(5,e);
	                    break;
	                case 6:
	                    string a,b,c,d,e,f;
	                    cin >> a >> b >> c >> d >> e >> f;
	                    tta.set(1,a);
	                    tta.set(2,b);
	                    tta.set(3,c);
	                    tta.set(4,d);
	                    tta.set(5,e);
	                    tta.set(6,f);
	                    break;


	            }
	    */
	}

    return 0;
}