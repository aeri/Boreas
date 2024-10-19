#include "LindaDriver.hpp"
#include "Tuple.hpp"

#include <chrono>

const int MAX_LONG_LINEA = 128;

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
    cout << "3. RD" << endl;
    cout << "4. RX" << endl;
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
	
    // Se presenta por pantalla el menú de opciones
    presentarMenu();

    // mientras no ha finalizado
    while(!stop)
	{
	    // Petición al usuario de tarea a pedir al cliente
	    cout << endl << "Introduzca operación: " << flush;
	    cin >> operacion;
	    string componente;

	    // si no desea parar la ejecución del cliente
	    if(operacion != 0)
		{
		    // Petición de dimensión
		    cout << "Introduzca dimensión: " << flush;
		    cin >> dimension;

		    // Validación de la dimensión de la tupla
		    while (cin.fail() || dimension < 1 || dimension > 6) {
		    	cin.clear();
        		cin.ignore(MAX_LONG_LINEA,'\n');
				cerr << "Dimensión no válida " << endl;
				cout << "Introduzca dimensión: " << flush;
				cin >> dimension;
		    }

		    
		    // Constructor de la tupla
		    Tuple tta(dimension);
		   
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
			    cout << "Se devuelve: " << LindaDriver.RD(tta).to_string() << endl;
			    auto end = std::chrono::system_clock::now();
			    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - init);
				
			    cout << "Operacion realizada en: " << elapsed.count() << " milisegundos." << endl;
		    }
		    else if(operacion == 4){
		    	bool found;
			    // el usuario pide hacer ReadNote
			    cout << "Se realiza ReadNote no bloqueante de " << tta.to_string() << endl;
			    auto init = std::chrono::system_clock::now();

			    string tupla = LindaDriver.RX(tta, found).to_string();

			    if (found){
			    	cout << "Se devuelve: " <<  tupla << endl;
			    }
			    else{
			    	cout << "Tupla no encontrada." << endl;
			    }
			    
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
