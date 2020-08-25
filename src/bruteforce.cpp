/*
 * Copyright (c) 2020 Naval Alcalá
 * Copyright (c) 2020 Rubén Rodríguez
 *
 * This file is part of Boreas.
 * Boreas is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Boreas is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Boreas.  If not, see <https://www.gnu.org/licenses/>.
 */

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
