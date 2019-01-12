#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include "LindaDriver.hpp"
#include "Socket.hpp"
#include "tuplas.hpp"

using namespace std;
/*Pre:s es un string que representa una tupla cuyas componentes están sepradas por comas
 *Post:Devuelve el número de componentes separadas por comas que hay en s
 */
int tamanyo(string s)
{
    stringstream ss(s);
    int dimension = 1;
    for(unsigned int i = 0; i < s.size(); i++)
	{
	    if(ss.get() == ',')
		{
		    ++dimension;
		}
	}
    return dimension;
}

/*Pre:ip es una direccion ip valida y p es un puerto válido
 *Post:Constructor de la clase LD
 */

LD::LD(string ip, string p) : Socket(ip, stoi(p))
{
    const int MAX_ATTEMPS = 10;
    int count = 0;

    do
	{
	    // Conexión con el servidor
	    socket_fd = Connect();
	    count++;

	    // Si error --> esperamos 1 segundo para reconectar
	    if(socket_fd == -1)
		{
		    this_thread::sleep_for(chrono::seconds(1));
		}
	}
    while(socket_fd == -1 && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_fd == -1)
	{
	    cout << "Error Grave" << endl;
	}
};


/*Pre:la dimension de t es >=1 && <=6
 *Post:Ha informado al servidor correspondiente la tupla sobre la que debe ejcutar PostNote 
 */

void LD::PN(Tupla t)
{
    const int MESSAGE_SIZE = 4001;

    string message = "PN:" + t.to_string();

    int send_bytes = Send(socket_fd, message);


    if(send_bytes == -1)
	{
	    cerr << "Error al enviar datos: " << strerror(errno) << endl;
	    // Cerramos el socket
	    Close(socket_fd);
	    exit(1);
	}
    cout << "Enviado PN" << endl;
    // Buffer para almacenar la respuesta, como char[]
    string buffer;

	int read_bytes = Recv(socket_fd, buffer, MESSAGE_SIZE);

	if (read_bytes > 0){
		cout << "\033[1;41m" << buffer << "\033[0m\n";
	}
	else{
		cerr << "ERROR" << endl;
	}
};


/*Pre:la dimension de t es >=1 && <=6
 *Post:Ha informado al servidor correspondiente la tupla sobre la que debe ejcutar RemoveNote y la devuelve 
 */


Tupla LD::RN(Tupla t)
{
    const int MESSAGE_SIZE = 4001;

    string message = "RN:" + t.to_string();

    int send_bytes = Send(socket_fd, message);

    if(send_bytes == -1)
	{
	    cerr << "Error al enviar datos: " << strerror(errno) << endl;
	    // Cerramos el socket
	    Close(socket_fd);
	    exit(1);
	}

    // Buffer para almacenar la respuesta, como char[]
    string buffer;
    int read_bytes = Recv(socket_fd, buffer, MESSAGE_SIZE);

    if(read_bytes == -1)
	{
	    string mensError(strerror(errno));
	    cerr << "Error al recibir datos: " + mensError + "\n";
	    // Cerramos los sockets
	    Close(socket_fd);
	}
	cout << "vuelta:" << buffer << endl;
    Tupla r(tamanyo(buffer));
    r.from_string(buffer);
    return r;
};


/*Pre:la dimension de t es >=1 && <=6
 *Post:Ha informado al servidor correspondiente la tupla sobre la que debe ejcutar ReadNote y la devuelve 
 */

Tupla LD::ReadN(Tupla t)
{
    const int MESSAGE_SIZE = 4001;

    string message = "ReadN:" + t.to_string();

    int send_bytes = Send(socket_fd, message);

    if(send_bytes == -1)
	{
	    cerr << "Error al enviar datos: " << strerror(errno) << endl;
	    // Cerramos el socket
	    Close(socket_fd);
	    exit(1);
	}

    // Buffer para almacenar la respuesta, como char[]
    string buffer;
    int read_bytes = Recv(socket_fd, buffer, MESSAGE_SIZE);

    if(read_bytes == -1)
	{
	    string mensError(strerror(errno));
	    cerr << "Error al recibir datos: " + mensError + "\n";
	    // Cerramos los sockets
	    Close(socket_fd);
	}
	cout << "vuelta:" << buffer << endl;
    Tupla r(tamanyo(buffer));
    r.from_string(buffer);
    return r;
};

/*Pre:
 *Post:Cierra la conexión con el cliente
 */

void LD::STOP()
{
    Send(socket_fd, "END OF SERVICE");
    Close(socket_fd);
};
