/*
 * ----------------------------------------------------------
 * -- Programación de sistemas concurrentes y distribuidos --
 * -- Trabajo práctico de Servidor Linda -------------------- 
 * -- Autores -----------------------------------------------
 * -- Daniel Naval Alcalá 739274 ----------------------------
 * -- Alejandro Omist Casado 739971 -------------------------
 * -- Rubén Rodríguez Esteban 737215 ------------------------
 * -- José Manuel Romero Clavería 740914 --------------------
 * ----------------------------------------------------------
 */

#include "LindaDriver.hpp"
#include "tuplas.hpp"

#include <chrono>

using namespace std;

/*
 * Pre: ---
 * Post: Muestra por pantalla un menu con las posibles opciones
 *       que puede realizar el usuario
 */
void presentarMenu(){
    cout << "Cliente interactivo Version 0.1" << endl;
    cout << "1. PN" << endl;
    cout << "2. RM" << endl;
    cout << "3. ReadN" << endl;
    cout << "0. Parar" << endl;
}


/*
 * Programa que refleja el comportamiento de un cliente interactivo.
 * Comienza solicitando el cṕdigo de operación por teclado al usuario,
 * puede introducir 1 para PostNote, 2 para RemoveNote, 3 para ReadNote
 * y 0 para terminar ejecución. Si el usuario introduce un código distinto 
 * del valor 0 pide la dimensión de la tupla y las componentes para formarla
 * al usario, y en caso contario, el cliente cierra conexión con el servidor Linda
 */
int main(int argc, char* argv[])
{
    // faltan argumentos en la invocación?
    if(argc < 2)
	{
	    cerr << "Invocar como:" << endl;
	    cerr << "   mainLindaDriver <IP_LS> <Port_LS>" << endl;
	    cerr << "      <IP_LS>: IP del servidor Linda" e< endl;
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
	
    // Se presenta por pantalla el menú de opciones
    presentarMenu();

    // mientras no ha finalizado
    while(!stop)
	{
	    // Petición al usuario de tarea a pedir al cliente
	    cout << endl << "Introduzca operación: " << flush;
	    cin >> operacion;

	    int i = 1;
	    string componente;

	    // si no desea parar la ejecución del cliente
	    if(operacion != 0)
		{
		    // Validación de la dimensión de la tupla
		    cout << "Introduzca dimensión: " << flush;
		    cin >> dimension;
		    
		    // Constructor de la tupla
		    Tupla tta(dimension);
		   
		    for (int i = 1; i <=dimension; i++){
			
			// Petición de componente de tupla al usuario
		 	cout << "Introduce la componente de la tupla " << i << ": " << flush;
		        cin >> componente;
			    
			// insertar componente en la tupla
			tta.set(i, componente);
		    } 

		    if(operacion == 1){
			    // el usuario pide hacer PostNote
			    cout << "Se realiza PostNote de " << tta.to_string() << endl;
			    auto init = std::chrono::system_clock::now();
			    LindaDriver.PN(tta);
			    auto end = std::chrono::system_clock::now();
			    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);
				
			    cout << "Operacion realizada en: " << elapsed.count() << " milisegundos." << endl;
		    }
		    else if(operacion == 2){
			    // el usuario pide hacer RemoveNote
			    cout << "Se realiza RemoveNote de " << tta.to_string() << endl;
			    auto init = std::chrono::system_clock::now();
			    cout << "Se devuelve: " << LindaDriver.RN(tta).to_string() << endl;
			    auto end = std::chrono::system_clock::now();
			    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);
				
				cout << "Operacion realizada en: " << elapsed.count() << " milisegundos." << endl;
		    }
		    else if(operacion == 3){
			    // el usuario pide hacer ReadNote
			    cout << "Se realiza ReadNote de " << tta.to_string() << endl;
			    auto init = std::chrono::system_clock::now();
			    cout << "Se devuelve: " << LindaDriver.ReadN(tta).to_string() << endl;
			    auto end = std::chrono::system_clock::now();
			    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);
				
			    cout << "Operacion realizada en: " << elapsed.count() << " milisegundos." << endl;
		    }	
		}
	    else{
		  // Se cierra conexión con el servidor Linda  
		  cout << "Ejecución finalizada" << endl;
		  LindaDriver.STOP();
		  stop = true;
	    }
	}

    return 0;
}
