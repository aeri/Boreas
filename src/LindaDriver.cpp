#include <iostream>
#include <iomanip>
#include <cstring>
#include "LindaDriver.hpp"
#include "Socket.hpp"

using namespace std;



LD::LD (string p, string i){
	port = p;
	ip = stoi(i,10);
	Socket socket(ip, port);


	const int MAX_ATTEMPS = 10;
	int count = 0;
	
	do {
	// Conexión con el servidor
    		socket_fd = socket.Connect();
    		count++;

	// Si error --> esperamos 1 segundo para reconectar
    		if(socket_fd == -1){
    		    this_thread::sleep_for(chrono::seconds(1));
    		}
    	} while(socket_fd == -1 && count < MAX_ATTEMPS);

	// Chequeamos si se ha realizado la conexión
   	 if(socket_fd == -1){
   		cout << "Error Grave" << endl;
   	 }

}

void LD::PN (Tupla t){
	const string MENS_FIN("END OF SERVICE");
	const int MESSAGE_SIZE = 4001;
	int send_bytes = socket.Send(socket_fd, "PN");
	if(send_bytes == -1){
		cerr << "Error al enviar datos: " << strerror(errno) << endl;
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}

	if(mensaje != MENS_FIN){
	    // Buffer para almacenar la respuesta, como char[]
	    string buffer;
	    int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
	}

	if (strcmp("OK",buffer) != 0){
		int send_bytes = socket.Send(socket_fd, t.to_string());
		if(send_bytes == -1){
			cerr << "Error al enviar datos: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}
	}

Tupla LD::RN (Tupla t){
	const string MENS_FIN("END OF SERVICE");
	const int MESSAGE_SIZE = 4001;
	int send_bytes = socket.Send(socket_fd, "RN");
	if(send_bytes == -1){
		cerr << "Error al enviar datos: " << strerror(errno) << endl;
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}

	if(mensaje != MENS_FIN){
	    // Buffer para almacenar la respuesta, como char[]
	    string buffer;
	    int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
	}

	if (strcmp("OK",buffer) != 0){
		int send_bytes = socket.Send(socket_fd, t.to_string());
		if(send_bytes == -1){
			cerr << "Error al enviar datos: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}
	}
	if(mensaje != MENS_FIN){
	    // Buffer para almacenar la respuesta, como char[]
	    string buffer;
	    int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
	}
	return from_string(buffer);
}

Tupla LD::ReadN (Tupla t){
	const string MENS_FIN("END OF SERVICE");
	const int MESSAGE_SIZE = 4001;
	int send_bytes = socket.Send(socket_fd, "ReadN");
	if(send_bytes == -1){
		cerr << "Error al enviar datos: " << strerror(errno) << endl;
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}

	if(mensaje != MENS_FIN){
	    // Buffer para almacenar la respuesta, como char[]
	    string buffer;
	    int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
	}

	if (strcmp("OK",buffer) != 0){
		int send_bytes = socket.Send(socket_fd, t.to_string());
		if(send_bytes == -1){
			cerr << "Error al enviar datos: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}
	}
	if(mensaje != MENS_FIN){
	    // Buffer para almacenar la respuesta, como char[]
	    string buffer;
	    int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
	}
	return from_string(buffer);
}