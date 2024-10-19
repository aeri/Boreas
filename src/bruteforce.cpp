#include "LindaDriver.hpp"
#include "Tuple.hpp"
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 2)
	{
	    cerr << "Invocar como:" << endl;
	    cerr << "   bruteforce <ipServer> <portServer>" << endl;
        cerr << "      <ipServer>: Número de tuplas a lanzar" << endl;
        cerr << "      <portServer>: Puerto del serivdor Linda" << endl;
	    return 1;
	}



    // un driver con los datos pasados en la invocación
    LD LindaDriver(argv[1], argv[2]);
    // La conexión con el servidor Linda ya está establecida

    cout << "Conexión establecida con éxito" << endl;

    //int dimension = rand() % 6 + 1;
    
    string entrada;
    Tuple ttp(6);
    string start="[a,b,c,d,e,";
    string stop="]";
    
    int i = 0;

    while (1){
        entrada =start+to_string(i)+stop;
        ttp.from_string(entrada);
        
        LindaDriver.PN(ttp);

        cout << "Tupla número: " << i << " insertada correctamente."  << endl;

        ++i;

    }
	
    LindaDriver.STOP();
    return 0;
}
