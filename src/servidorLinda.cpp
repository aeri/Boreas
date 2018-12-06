#include <thread>
#include "Socket.hpp"
#include <queue>
#include <string>
#include <iostream>

#include <mutex>


const int N = 10;

int main(int argc, char *argv[]) {
	
	string SERVER_ADDRESS = "localhost";
	int SERVER_PORT=atoi(argv[1]);
	thread cliente[N];
	int client_fd[N];

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_PORT);

	
	// Bind
	int socket_fd =socket.Bind();
	if (socket_fd == -1) {
		string mensError(strerror(errno));
    		cerr << "Error en el bind: " + mensError + "\n";
		exit(1);
	}

	// Listen
  int max_connections = N;
	int error_code = socket.Listen(max_connections);
	if (error_code == -1) {
		string mensError(strerror(errno));
    		cerr << "Error en el listen: " + mensError + "\n";
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}

	//thread
	int contados=0;
	int i =0;
	
	//Atendemos a las peticiones de los clientes
	while(i<max_connections && !finTOTAL){
			// Accept
			client_fd[i] = socket.Accept();

			if(client_fd[i] == -1) {
				string mensError(strerror(errno));
	    			cerr << "Error en el accept: " + mensError + "\n";
				// Cerramos el socket
				socket.Close(socket_fd);
				exit(1);
			}
			if(!finTOTAL){
				string aux="Lanzo thread nuevo cliente "+ to_string(i)+"\n";

				cout << aux;
				thread(&servCliente, ref(socket), client_fd[i]).detach();
				aux="Nuevo cliente " + to_string(i) + " aceptado" + "\n";
				cout << aux;

				contados++;
				i++;
			}
			else{
				string aux="Se va a Finalizar el programa\n";
				cout<<aux;
			}
	}
	//join();
}