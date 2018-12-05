//*****************************************************************
// File:   mainLindaDriver.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2016
// Coms:   Esqueleto para prueba del TP-6
//         invocar mediante 
//            mainLindaDriver IP_LindaServer Port_LindaServer ...
//*****************************************************************

...

#include "LindaDriver.hpp"
#include "Tuplas.hpp"

using namespace std;

//-----------------------------------------------------
int main(int argc, char* argv[]) {

    //faltan argumentos en la invocación?
    if (argc < ... ) { 
        cerr << "Invocar como:" << endl;
        cerr << "   mainLindaDriver <IP_LS> <Port_LS> ..." << endl;
        cerr << "      <IP_LS>: IP del servidor Linda" << endl;
        cerr << "      <Port_LS>: puerto del servidor Linda" << endl;
        return 1;
    }

    // un driver con los datos pasados en la invocación
    LindaDriver LD(argv[1], argv[2], ...);
    // La conexión con el servidor Linda ya está establecido

    // Varias formas de construir tuplas
    Tupla t1("1","mi casa","árbol");  // 3 elementos
    Tupla t2("1000");
    Tupla t3("aprieta","el","pan","45","34","88");
    Tupla t4("aprieta","fuerte","pan","tt","34","pan");
    // insertar las tuplas en linda: varios PostNote
    LD.PN(t1);
    LD.PN(t2);
    LD.PN(t3);
    LD.PN(t3);
    LD.PN(t3);
    LD.PN(t4);
    
    //muestra "mi casa" por stdout
    cout << t1.get(2) << endl;
    // t3.to_string() devuelve el string "[aprieta,el,pan,45,34,88]"
    string serial = t3.to_string();
    cout << serial << endl;
    // las componentes de t3 tomarán, respectivamente,
    // los valores "a","b","c","45","34","pan"
    t3.from_string("[a,b,c,45,34,pan]");
    // mostrará [a,b,c,45,34,pan] por stdout
    cout << t3.to_string() << endl; 
    ...

    // Crea una tupla de 3 elementos "".
    // Equivalente a Tupla t5("","","")
    Tupla t5(3); 
    t5.set(2, "hola");
    t5.set(3, "Mundo");
    LD.PN(t5);
    // mostrará [,hola,Mundo] por stdout
    cout << t5.to_string() << endl; 
    // informará de que tiene 3 elementos
    cout << "t5 tiene " << t5.size() << " elementos" << endl; 
    ...

    // Un patrón no es otra cosa que una tupla con 
    // la posibilidad de contener el comodín "?" en una o más posiciones
    // "?" no puede ser un valor de componente de tupla. Para ello se 
    // tendría que representar mediante "??". Creamos dos patrones
    Tupla p1("?X"); 
    Tupla p2("a","?X","c","?Y","34","?Z");
    // Dos nuevas tuplas, de tamaño 1 y 6, respectivamente
    Tupla res1(1), 
          res2(p2.size());
    // ejemplos de RemoveNote
    res1 = LD.RN(p1); // res1 tomará él valor que tenía t2
    res2 = LD.RN(p2); // res2 tomará el valor que tenía t3
    cout << res1.to_string() << endl; //mostrará [1000]
    cout << res2.to_string() << endl; //mostrará [a,b,c,45,34,pan]
    ... 

    // ¿Si necesitamos un array de tuplas?
    // Tupla v[2]; // NO permitido: no hay constructor por defecto
    Tupla *v[2];
    v[0] = new Tupla("Juan", "1000");
    v[1] = new Tupla("Luisa", "1000", "enero");
    ...
    delete v[0];
    delete v[1];

    return 0;
}