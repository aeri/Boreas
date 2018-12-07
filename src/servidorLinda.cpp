#include <thread>
#include "Socket.hpp"
#include <queue>
#include <string>
#include <iostream>

#include <mutex>

bool STOP = false;
const int N = 10;

void servCliente(Socket& soc, int client_fd) {
	
	// Buffer para recibir el mensaje

	char MENS_FIN[]="END OF SERVICE";
	// Buffer para recibir el mensaje
	int length = 100;
	char buffer[length];

	bool out = false; // Inicialmente no salir del bucle
	while(!out) {
		// Recibimos el mensaje del cliente
		int rcv_bytes = soc.Recv(client_fd,buffer,length);
		if (rcv_bytes == -1) {
			string mensError(strerror(errno));
    		cerr << "Error al recibir datos: " + mensError + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
		}

		cout << "Mensaje recibido: " << buffer << endl;
		int send_bytes = soc.Send(client_fd, "OK");



		rcv_bytes = soc.Recv(client_fd,buffer,length);
		if (rcv_bytes == -1) {
			string mensError(strerror(errno));
    		cerr << "Error al recibir datos: " + mensError + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
		}

		cout << "Tupla: " << buffer << endl;
		// Si recibimos "END OF SERVICE" --> Fin de la comunicación
	}

	soc.Close(client_fd);
}


int main(int argc, char *argv[]) {
	
 if (argc < 2 ) { 
        cerr << "Invocar como:" << endl;
        cerr << "   servidorLinda <Port_LS>" << endl;
        cerr << "      <Port_LS>: puerto del servidor Linda" << endl;
        return 1;
    }


	string SERVER_ADDRESS = "localhost";
	int SERVER_PORT=atoi(argv[1]);
	thread cliente[N];
	int client_fd[N];

	cout << "Iniciando servidor Linda en puerto " << SERVER_PORT << endl;



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
	while(i<max_connections && !STOP){
			// Accept
			client_fd[i] = socket.Accept();

			if(client_fd[i] == -1) {
				string mensError(strerror(errno));
	    			cerr << "Error en el accept: " + mensError + "\n";
				// Cerramos el socket
				socket.Close(socket_fd);
				exit(1);
			}
			if(!STOP){
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
	return 0;
}