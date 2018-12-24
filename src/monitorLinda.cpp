#include <iomanip>
#include <queue>
#include <thread>
#include "monitorLinda.hpp"

using namespace std;

const int MAX_ATTEMPS = 10;

//-----------------------------------------------------
MonitorLinda::MonitorLinda()
{
    tupleSpace1.primero = nullptr;
    tupleSpace2.primero = nullptr;
    tupleSpace3.primero = nullptr;
    tupleSpace4.primero = nullptr;
    tupleSpace5.primero = nullptr;
    tupleSpace6.primero = nullptr;
};

MonitorLinda::~MonitorLinda(){

};

//----------------- Destructor
/*
void MonitorLinda::RemoveNote(Tupla t)
{
    unique_lock<mutex> lck(mtxMonitor);

    



};
*/

bool sonIguales(string a, string b)
{
    if(b[0] == '?' && b[1] >= 'A' && b[1] <= 'Z')
	{
	    return true;
	}
    else
	{
	    return a == b;
	}
}

void MonitorLinda::PostNote(Tupla t)
{
    unique_lock<mutex> lck(mtxMonitor);
    cout << "////MONITOR ESCRIBE" << endl;

    bbdd::Nodo* np = new bbdd::Nodo;

    int dimension = t.size();
    cout << "DIMENSION: " << dimension << endl;
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
	    cout << "Error en la dimensión de la tupla" << endl;
	}

    for(int i = 1; i <= dimension; ++i)
	{
	    np->valor = t.get(i);
	    cout << np->valor << endl;
	    if(i != dimension)
		{
		    cout << "new!" << endl;
		    np->sigComp = new bbdd::Nodo;
		    np = np->sigComp;
		}
	    else
		{
		    np->sigComp = nullptr;
		}
	}
    cout << "////MONITOR ESCRIBE ACABA" << endl;

    /*
        bbdd::Nodo* taux = tupleSpace.primero;

        while (taux != nullptr){
            cout << taux->valor << endl;
            taux = taux->sigComp;
        }

    */
    switch(dimension)
	{
	case 1:
	    hay_tupla1.notify_all();
	    break;
	case 2:
	    hay_tupla2.notify_all();
	    break;
	case 3:
	    hay_tupla3.notify_all();
	    break;
	case 4:
	    hay_tupla4.notify_all();
	    break;
	case 5:
	    hay_tupla5.notify_all();
	    break;
	case 6:
	    hay_tupla6.notify_all();
	    break;
	default:
	    cout << "Error en la dimensión de la tupla" << endl;
	}
}
void MonitorLinda::RemoveNote(Tupla t, Tupla& r)
{
    unique_lock<mutex> lck(mtxMonitor);

    cout << "////MONITOR BORRA" << endl;

    bbdd::Nodo* fila;
    bbdd::Nodo* columna;
    bbdd::Nodo* aux;

    int dimension = t.size();

    bbdd::Nodo* store;
    switch(dimension)
	{
	case 1:
	    store = tupleSpace1.primero;
	    break;
	case 2:
	    store = tupleSpace2.primero;
	    break;
	case 3:
	    store = tupleSpace3.primero;
	    break;
	case 4:
	    store = tupleSpace4.primero;
	    break;
	case 5:
	    store = tupleSpace5.primero;
	    break;
	case 6:
	    store = tupleSpace6.primero;
	    break;
	default:
	    store = nullptr;
	    cout << "Error en la dimensión de la tupla" << endl;
	}

    fila = tupleSpace3.primero;
    columna = tupleSpace3.primero;
    aux = tupleSpace3.primero;

    bool encontrado = false;
    bool sigue_buscando = true;
    int i;

    bool heBuscado = false;
    while(!encontrado)
	{
	    while(heBuscado || columna == nullptr)
		{
            cout << "XXXX Tupla no encontrada XXXX" << endl;
		    switch(dimension)
			{
			case 1:
			    hay_tupla1.wait(lck);
			    break;
			case 2:
			    hay_tupla2.wait(lck);
			    break;
			case 3:
			    cout << "WAIT TUPLA 3" << endl;
			    hay_tupla3.wait(lck);
                cout << "DESPERTADO TUPLA 3" << endl;
			    break;
			case 4:
			    hay_tupla4.wait(lck);
			    break;
			case 5:
			    hay_tupla5.wait(lck);
			    break;
			case 6:
			    hay_tupla6.wait(lck);
			    break;
			default:
			    cout << "Error en la dimensión de la tupla" << endl;
			}
		    fila = tupleSpace3.primero;
		    columna = tupleSpace3.primero;
		    aux = tupleSpace3.primero;
		}
	    // toda la mierda de abajo

	    while(!encontrado && columna != nullptr)
		{
		    sigue_buscando = true;
		    i = 1;
		    cout << "bucle de arriba" << endl;
		    while(sigue_buscando && fila != nullptr)
			{
			    sigue_buscando = sonIguales(fila->valor, t.get(i));
			    cout << fila->valor << " == " << t.get(i) << endl;
			    cout << boolalpha << sigue_buscando << endl;

			    if(sigue_buscando)
				{
				    r.set(i, fila->valor);
                    cout << fila->valor << endl;
				    fila = fila->sigComp;
				    ++i;
				}
			}
		    if(fila == nullptr && sigue_buscando)
			{
			    encontrado = true;
			    aux->sigTupla = columna->sigTupla;

			    // BORRADO
			    bbdd::Nodo* saux;
			    while(columna != nullptr)
				{
				    saux = columna;
				    columna = columna->sigComp;
				    cout << "Se borra la componente: " << saux->valor << endl;
				    delete(saux);
				}
			}
		    else
			{
			    aux = columna;
			    columna = columna->sigTupla;
			    fila = columna;
			}
		}
	}
    heBuscado = true;
}
void MonitorLinda::ReadNote(Tupla t, Tupla& r)
{
    unique_lock<mutex> lck(mtxMonitor);

    cout << "////MONITOR LEE" << endl;

    bbdd::Nodo* fila;
    bbdd::Nodo* columna;

    bbdd::Nodo* store;

    int dimension = t.size();

    switch(dimension)
	{
	case 1:
	    store = tupleSpace1.primero;
	    break;
	case 2:
	    store = tupleSpace2.primero;
	    break;
	case 3:
	    store = tupleSpace3.primero;
	    break;
	case 4:
	    store = tupleSpace4.primero;
	    break;
	case 5:
	    store = tupleSpace5.primero;
	    break;
	case 6:
	    store = tupleSpace6.primero;
	    break;
	default:
	    store = nullptr;
	    cout << "Error en la dimensión de la tupla" << endl;
	}

    fila = store;
    columna = store;

    bool encontrado = false;
    bool sigue_buscando;
    int i;
    bool heBuscado = false;
    while(!encontrado)
	{
	    while(heBuscado || columna == nullptr)
		{
		    switch(dimension)
			{
			case 1:
			    hay_tupla1.wait(lck);
			    break;
			case 2:
			    hay_tupla2.wait(lck);
			    break;
			case 3:
			    hay_tupla3.wait(lck);
			    break;
			case 4:
			    hay_tupla4.wait(lck);
			    break;
			case 5:
			    hay_tupla5.wait(lck);
			    break;
			case 6:
			    hay_tupla6.wait(lck);
			    break;
			default:
			    cout << "Error en la dimensión de la tupla" << endl;
			}
		    fila = store;
		    columna = store;
		}

	    while(!encontrado && columna != nullptr)
		{
		    sigue_buscando = true;
		    i = 1;
		    cout << "bucle de arriba" << endl;
		    while(sigue_buscando && fila != nullptr)
			{
			    sigue_buscando = sonIguales(fila->valor, t.get(i));
			    cout << fila->valor << " == " << t.get(i) << endl;
			    cout << boolalpha << sigue_buscando << endl;

			    if(sigue_buscando)
				{
				    r.set(i, fila->valor);
				    fila = fila->sigComp;
				    ++i;
				}
			}
		    encontrado = (fila == nullptr && sigue_buscando);

		    columna = columna->sigTupla;
		    fila = columna;
		}
	    if(!encontrado)
		{
		    cout << "XXXX Tupla no encontrada XXXX" << endl;
		}
	}
}