#include <queue>
#include <thread>
#include "monitorLinda.hpp"
#include <iomanip>

using namespace std;

const int MAX_ATTEMPS = 10;

//-----------------------------------------------------
MonitorLinda::MonitorLinda()
{

    tupleSpace.primero=nullptr;
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

bool sonIguales(string a, string b){
    if (b[0] == '?' && b[1] > 'A' && b[1] < 'Z'){
        return true;
    }
    else{
        return a == b;
    }
}

void MonitorLinda::PostNote(Tupla t){
    unique_lock<mutex> lck(mtxMonitor);
    cout << "////MONITOR ESCRIBE" << endl; 

     bbdd::Nodo* np = new bbdd::Nodo;

     np->sigTupla = tupleSpace.primero;

     tupleSpace.primero = np;


     for (int i = 1; i<= t.size(); ++i){
        np->valor = t.get(i);
        cout << np->valor << endl;
        if (i != t.size()){
            cout << "new!" << endl;
            np->sigComp = new bbdd::Nodo;
            np  = np ->sigComp;
            
        }
        else{
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
    //hay_tupla.notify_all();

}
void MonitorLinda::RemoveNote(Tupla t, Tupla& r){
    unique_lock<mutex> lck(mtxMonitor);

    cout << "////MONITOR BORRA" << endl; 
    
     bbdd::Nodo* fila = tupleSpace.primero;
     bbdd::Nodo* columna = tupleSpace.primero;
     bbdd::Nodo* aux = tupleSpace.primero;

     bool encontrado = false;
     bool sigue_buscando;
     int i;

     while (!encontrado && columna != nullptr){
        sigue_buscando = true;
        i = 1;
        cout << "bucle de arriba" << endl;
        while (sigue_buscando && fila != nullptr){
            sigue_buscando = sonIguales(fila->valor, t.get(i));
            cout << fila->valor << " == " << t.get(i) << endl;
            cout << boolalpha << sigue_buscando << endl;

            if (sigue_buscando){
                r.set(i, fila->valor);
                fila = fila->sigComp;
                ++i;
            }

        }
        if (fila == nullptr && sigue_buscando){
            encontrado = true;
            aux->sigTupla = columna ->sigTupla;

            //BORRADO
            

            bbdd::Nodo* saux;
            while (columna != nullptr){
                saux = columna;
                columna = columna->sigComp;
                cout << "Se borra la componente: " 
                << saux->valor << endl;
                delete (saux);
            }
        }
        else{
            aux = columna;
            columna = columna->sigTupla;
            fila = columna;
        }

     }
     if (!encontrado){
        cout << "XXXX Tupla no encontrada XXXX" << endl;
     }

}
void MonitorLinda::ReadNote(Tupla t, Tupla& r){
    unique_lock<mutex> lck(mtxMonitor);

    cout << "////MONITOR LEE" << endl; 
    
//    hay_tupla.wait(lck);

     bbdd::Nodo* fila = tupleSpace.primero;
     bbdd::Nodo* columna = tupleSpace.primero;
     bool encontrado = false;
     bool sigue_buscando;
     int i;

     while (!encontrado && columna != nullptr){
        sigue_buscando = true;
        i = 1;
        cout << "bucle de arriba" << endl;
        while (sigue_buscando && fila != nullptr){
            sigue_buscando = sonIguales(fila->valor, t.get(i));
            cout << fila->valor << " == " << t.get(i) << endl;
            cout << boolalpha << sigue_buscando << endl;

            if (sigue_buscando){
                r.set(i, fila->valor);
                fila = fila->sigComp;
                ++i;
            }

        }
        encontrado=(fila == nullptr && sigue_buscando);
        
        columna = columna->sigTupla;
        fila = columna;
     }
     if (!encontrado){
        cout << "XXXX Tupla no encontrada XXXX" << endl;
     }

}