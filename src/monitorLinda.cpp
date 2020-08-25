/*
 * ----------------------------------------------------------
 * -- Programación de sistemas concurrentes y distribuidos --
 * -- Trabajo práctico : Servidor Linda ---------------------
 * -- Autores y NIP -----------------------------------------
 * -- Daniel Naval Alcalá  739274 ---------------------------
 * -- Alejandro Omist Casado 737791 -------------------------
 * -- Rubén Rodríguez Esteban 737215 ------------------------
 * -- José Manuel Romero Clavería 740914 --------------------
 * ----------------------------------------------------------
 */

/*
 * Fichero de implementación del monitor de almacenamiento de tuplas.
 */

#include <iomanip>
#include <queue>
#include <thread>
#include "monitorLinda.hpp"

using namespace std;

const int NUM_MATRIX = 6;

//-----------------------------------------------------

/*
 * Pre: ---
 * Post:Constructor de la clase monitorLinda
 */
MonitorLinda::MonitorLinda(){
    tupleSpace1.primero = nullptr;
    tupleSpace2.primero = nullptr;
    tupleSpace3.primero = nullptr;
    tupleSpace4.primero = nullptr;
    tupleSpace5.primero = nullptr;
    tupleSpace6.primero = nullptr;
};


/*
 * Pre:
 * Post:Destructor de la clase monitorLinda
 */
MonitorLinda::~MonitorLinda(){

	bbdd::Nodo* aux;
	// Recorrido de todos los espacios de tuplas
	for (int i = 1; i <= NUM_MATRIX; ++i){
		switch (i){
		        // Se guarda en <<aux>> el puntero a la matriz que se va a borrar 
			case 1: aux=tupleSpace1.primero; break;
			case 2: aux=tupleSpace2.primero; break;
			case 3: aux=tupleSpace3.primero; break;
			case 4: aux=tupleSpace4.primero; break;
			case 5: aux=tupleSpace5.primero; break;
			case 6: aux=tupleSpace6.primero; break;
			default : 
				// Caso de error
				aux = nullptr; 
				cerr << "mon-error: tuple space > 6" << endl;
		}
		
		// Determinación de la componente siguiente de la tupla 
		bbdd::Nodo* sig = aux ->sigComp;
		bbdd::Nodo* del;
		
		// Borrado de todas las tuplas (por columnas)
		while (aux != nullptr){
			//Borrado de la tupla componente a componente (por filas)
			while (sig != nullptr){
				del = sig -> sigComp;
				delete (sig);
				sig = del;		
			}

			// Almacenamiento de la siguiente tupla a borrar
			// Borrado de la primera componente de la tupla 
			del = aux -> sigTupla;
			delete (aux);
			aux = del;
		}
	}
};



/*
 * Pre: <<a>> y <<b>> son dos strings que respetan el formato de las tuplas ["",""]
 * Post: Devuelve <<true>> si <<a>> y <<b>> tienen las mismas componentes, teninedo en cuenta
 *       que una componente ?x coincide con cualquier valor. Devuelve <<false>> en caso contario
 */
bool sonIguales(string a, string b) {
    	if(b[0] == '?' && b[1] >= 'A' && b[1] <= 'Z')
	{
	    return true;
	}
    	else
	{
	    return a == b;
	}
}


/*
 * Pre: La dimensión de la tupla t es >=1 && <=6
 * Post: Añade a la matriz correspondiente la tupla t
 */
void MonitorLinda::PostNote(Tuple t){
    unique_lock<mutex> lck(mtxMonitor);
   

    bbdd::Nodo* np = new bbdd::Nodo;

    // Obtención de la dimensión de la tupla
    int dimension = t.size();
    
    // Determinación en que matriz se va a insertar la tupla <<t>>
    // mandada por el cliente
    switch(dimension)
	{
	case 1:
	    np->sigTupla = tupleSpace1.primero;
	    tupleSpace1.primero = np;
	    break;
	case 2:
	    np->sigTupla = tupleSpace2.primero;
	    tupleSpace2.primero = np;
	    break;
	case 3:
	    np->sigTupla = tupleSpace3.primero;
	    tupleSpace3.primero = np;
	    break;
	case 4:
	    np->sigTupla = tupleSpace4.primero;
	    tupleSpace4.primero = np;
	    break;
	case 5:
	    np->sigTupla = tupleSpace5.primero;
	    tupleSpace5.primero = np;
	    break;
	case 6:
	    np->sigTupla = tupleSpace6.primero;
	    tupleSpace6.primero = np;
	    break;
	default:
	    cerr << "mon-error: tuple dimension > 6 in PN" << endl;
	}
    
    // Recorrido de las componentes de la tupla
    // Se asigna a cada fila de la matriz la componente i-ésima de la tupla
    for(int i = 1; i <= dimension; ++i)
	{
	    // Asignación de la componente
	    np->valor = t.get(i);
	    
	    if(i != dimension)
		{
		    // Reserva de memoria para la siguiente componente
		    // Desplazamiento al nuevo puntero
		    np->sigComp = new bbdd::Nodo;
		    np = np->sigComp;
		}
	    else
		{
		    // Final de la tupla 
		    np->sigComp = nullptr;
		}
	}
    
    // Tras añadir la tupla <<t>> se avisa a los clientes bloqueados
    // que están esperando una tupla de dimensión igual a la de la tupla <<t>>
    switch(dimension)
	{
	case 1:
	    // Aviso a los clientes que han pedido tupla de dimensión 1
	    hay_tupla1.notify_all();
	    break;
	case 2:
	    // Avisao a los clientes que han pedido tupla de dimensión 2
	    hay_tupla2.notify_all();
	    break;
	case 3:
	    // Aviso a los clientes que han pedido tupla de dimensión 3	    
	    hay_tupla3.notify_all();
	    break;
	case 4:
	    // Aviso a los clientes que han pedido tupla de dimensión 4
	    hay_tupla4.notify_all();
	    break;
	case 5:
	    // Aviso a los clientes que han pedido tupla de dimensión 5
	    hay_tupla5.notify_all();
	    break;
	case 6:
	    // Aviso a los clientes que han pedido tupla de dimensión 6
	    hay_tupla6.notify_all();
	    break;
	default:
	    // Error
	    cerr << "mon-error: failure at notify: tuple dimension > 6" << endl;
	}
}

/*
 * Pre: La dimensión de la tupla t es >=1 && <=6 
 * Post: Busca la tupla t en la matriz correspondiente, la guarda en la tupla <<r>> y la borra
 *       de la matriz de tuplas correspondiente
 */
void MonitorLinda::RemoveNote(Tuple t, Tuple& r){
    unique_lock<mutex> lck(mtxMonitor);

    // Punteros auxiliares
    bbdd::Nodo* fila;
    bbdd::Nodo* columna;
    bbdd::Nodo* aux;

    bbdd::Nodo* pr;

    // Obtención de la dimensión
    const int dimension = t.size();

    // A partir de la dimensión de la tupla <<t>> se 
    // determina la matriz de la que se va a extrarer
    switch(dimension)
	{
	case 1:
	    fila = tupleSpace1.primero;
	    break;
	case 2:
	    fila = tupleSpace2.primero;
	    break;
	case 3:
	    fila = tupleSpace3.primero;
	    break;
	case 4:
	    fila = tupleSpace4.primero;
	    break;
	case 5:
	    fila = tupleSpace5.primero;
	    break;
	case 6:
	    fila = tupleSpace6.primero;
	    break;
	default:
	    fila = nullptr;
	    cerr << "mon-error: failure at get matrix (RN): tuple dimension > 6" << endl;
	}

    // Asignación de punteros al punto de inicio de búsqueda
    columna = fila;
    aux = fila;
    pr = fila;


    bool encontrado = false;		// matriz encontrada
    bool sigue_buscando = true;		// matrices iguales
    int i;

    bool heBuscado = false;		// ningún cliente ha solicitado la tupla <<t>>
	
    // la tupla no se ha encontrado todavía en la matriz
    while(!encontrado)
	{
	    // mientras no quedan tuplas por recorrer en la matriz y
	    // no esté en posesión de otro cliente
	    while(heBuscado || columna == nullptr)
	    {
		    // La tupla no está disponible 
		    cout << "mon-info: tuple not found in tuples space (RN)" << endl;
		    // Se duerme el cliente en cola la asociada a la variable condición correcta
		    // Se determina en función de la dimensión de la tupla que ha pedido el cliente
		    switch(dimension)
			{
			case 1:  
			    hay_tupla1.wait(lck);
			    fila = tupleSpace1.primero;
			    break;
			case 2:
			    hay_tupla2.wait(lck);
			    fila = tupleSpace2.primero;
			    break;
			case 3:
			    hay_tupla3.wait(lck);
			    fila = tupleSpace3.primero;
			    break;
			case 4:
			    hay_tupla4.wait(lck);
			    fila = tupleSpace4.primero;
			    break;
			case 5:
			    hay_tupla5.wait(lck);
			    fila = tupleSpace5.primero;
			    break;
			case 6:
			    hay_tupla6.wait(lck);
			    fila = tupleSpace6.primero;
			    break;
			default:
			    cerr << "mon-error: failure at wait (RN): tuple dimension > 6" << endl;
			}
		    columna = fila;
		    aux = fila;
		    pr = fila;
		}
 
	    // La tupla está disponible y hay que econtrarla
	    while(!encontrado && columna != nullptr)
		{
		    sigue_buscando = true;
		    i = 1;
		    
		    //Bucle de búsqueda de tupla
		    while(sigue_buscando && fila != nullptr)
			{
			    // Evaluamos la componente de la tupla con la actual
			    sigue_buscando = sonIguales(fila->valor, t.get(i));
			    
			    // Si son iguales puede ser la tupla que el cliente ha pedido
			    if(sigue_buscando)
				{
				    // Inserción de la componente en la tupla a enviar al cliente 
				    // Desplazamiento a la siguiente componente
				    r.set(i, fila->valor);
				    fila = fila->sigComp;
				    ++i;
				}
			}
		    if(fila == nullptr && sigue_buscando)
			{
			    encontrado = true;
			    aux->sigTupla = columna->sigTupla;

			    /* Se comienza el borrado de elementos
			    	- Si el elemento a borrar el primero hay que tener en cuenta
			    		que el puntero a primero tendrá que actualizarse por
			    		el valor que tenga el puntero al siguiente que apunta
			    */

			    // Tratamiento especial si se borra la primera tupla
			    if (columna == pr){
			    	cout << "mon-info: deleting first element of tuples space" << endl;
				// Se debe desplazar el puntero de inicio de la matriz a la 
				// tupla siguiente para no perder los restantes
			    	switch(dimension)
					{
					case 1:
						tupleSpace1.primero = columna->sigTupla;
					    break;
					case 2:
						tupleSpace2.primero = columna->sigTupla;
					    break;
					case 3:
						tupleSpace3.primero = columna->sigTupla;
					    break;
					case 4:
						tupleSpace4.primero = columna->sigTupla;
					    break;
					case 5:
						tupleSpace5.primero = columna->sigTupla;
					    break;
					case 6:
						tupleSpace6.primero = columna->sigTupla;
					    break;
					default:
					    cerr << "mon-error: failure at delete first element: tuple dimension > 6" << endl;
					}

			    }
			    // Borrado de la tupla <<t>> pedia por el cliente en la matriz
			    bbdd::Nodo* saux;
			    while(columna != nullptr)
				{
				    saux = columna;
				    columna = columna->sigComp;
				    //cout << "Se borra la componente: " << saux->valor << endl;
				    delete(saux);
				    saux = nullptr;
				}
			}
		    else
			{
			    // si no coincide la primera componente de la tupla se pasa a la siguiente
			    // porque no tiene sentido seguir buscando
			    aux = columna;
			    columna = columna->sigTupla;
			    fila = columna;
			}
		}
	}
    // Ha habido un cliente que ha buscado esa tupla
    heBuscado = true;
}

/*Pre:la dimensión de la tupla t es >=1 && <=6
 *Post:Busca la tupla t en la matriz correspondiente y actualiza el valor de dicha tupla
 */

void MonitorLinda::ReadNote(Tuple t, Tuple& r, bool locked){

    unique_lock<mutex> lck(mtxMonitor);

    // Punteros auxiliares
    bbdd::Nodo* fila;
    bbdd::Nodo* columna;

    // Obtención de la dimensión de la tupla
    int dimension = t.size();

    // Determinación de en que matriz se va a trabajar en función de
    // la dimensión de la tupla
    switch(dimension)
	{
	case 1:
	    fila = tupleSpace1.primero;
	    break;
	case 2:
	    fila = tupleSpace2.primero;
	    break;
	case 3:
	    fila = tupleSpace3.primero;
	    break;
	case 4:
	    fila = tupleSpace4.primero;
	    break;
	case 5:
	    fila = tupleSpace5.primero;
	    break;
	case 6:
	    fila = tupleSpace6.primero;
	    break;
	default:
	    fila = nullptr;
	    cerr << "mon-error: failure at get matrix (ReadN): tuple dimension > 6" << endl;
	}

    columna = fila;

    bool encontrado = false;		// tupla encontrada
    bool sigue_buscando;		// se continua la búsqueda
    int i;
    bool heBuscado = false;		// la tupla no esta en posesión de ningún cliente
	
    // mientras no la haya encontrado
    while(!encontrado)
	{
	    // mientras la tupla <<t>>  esté en posesión de ningún cliente o
	    // no halla tuplas en la matriz el cliente se espera
	    while(heBuscado || columna == nullptr)
		{
			if (locked){
			    // gestión del bloqueo del cliente en la cola asociada a la 
			    // variable condición correspondiente

			    // La tupla no se ha encontrado 
			    cout << "mon-info: tuple not found in tuples space (RD/RX)" << endl;
			    
			    switch(dimension)
				{
				case 1:
				    hay_tupla1.wait(lck);
				    fila = tupleSpace1.primero;
				    break;
				case 2:
				    hay_tupla2.wait(lck);
				    fila = tupleSpace2.primero;
				    break;
				case 3:
				    hay_tupla3.wait(lck);
				    fila = tupleSpace3.primero;
				    break;
				case 4:
				    hay_tupla4.wait(lck);
				    fila = tupleSpace4.primero;
				    break;
				case 5:
				    hay_tupla5.wait(lck);
				    fila = tupleSpace5.primero;
				    break;
				case 6:
				    hay_tupla6.wait(lck);
				    fila = tupleSpace6.primero;
				    break;
				default:
				    cerr << "mon-error: failure at wait (RD/RX): tuple dimension > 6" << endl;
				}
			    columna = fila;
			}
			else{
				r.set(1,"NOT_FOUND");
				encontrado = true;
				break;
			}
		}

	    // la tupla está en la matriz y no está en posesión de ningún otro cliente
	    // Se busca la tupla <<t>> en la matriz
	    while(!encontrado && columna != nullptr)
		{
		    // Quedan tuplas por recorrer
		    sigue_buscando = true;
		    i = 1;
		    while(sigue_buscando && fila != nullptr)
			{
			    // Comprobación de si coninciden las componentes 
			    sigue_buscando = sonIguales(fila->valor, t.get(i));
			    if(sigue_buscando)
				{
				    // Inserción de la componente
				    // Desplazamiento a la componente siguiente
				    r.set(i, fila->valor);
				    fila = fila->sigComp;
				    ++i;
				}
			}
		    // Verificación de si la tupla se ha encntrado
		    encontrado = (fila == nullptr && sigue_buscando);

		    // Se avanza a la siguiente tupla
		    columna = columna->sigTupla;
		    fila = columna;
		}
	}
	heBuscado = true;
}
