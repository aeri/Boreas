//******************************************************************
// File:   ServidorMulticliente.cpp
// Author: PSCD-Unizar
// Date:   Noviembre 2015
// Coms:   Ejemplo de servidor multicliente con comunicación síncrona mediante sockets
//*****************************************************************

#include "Socket.hpp"
#include "tuplas.hpp"
#include "monitorLinda.hpp"
#include <iostream>
#include <thread>
#include <sstream>
#include <cstring> //manejo de cadenas tipo C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;



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
//-------------------------------------------------------------
// Cuenta el número de vocales existentes en un mensaje
// PRE: 
// POST: Devuelve el número de vocales existentes en el mensaje 'message'
//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd, MonitorLinda& ML) {
	char MENS_FIN[]="END OF SERVICE";
	// Buffer para recibir el mensaje
	int length = 100;
	char buffer[length];

	bool out = false; // Inicialmente no salir del bucle
	while(!out) {
		// Recibimos el mensaje del cliente
		cout << "En espera a recibir datos del servidorLinda" << endl;
		int rcv_bytes = soc.Recv(client_fd,buffer,length);
		cout << buffer << endl;
		if (rcv_bytes == -1) {
			string mensError(strerror(errno));
    		cerr << "Error al recibir datos: " + mensError + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
		}

		char* operacion = strtok (buffer,":");
		char* tupla = strtok (NULL, ":");
		int ssize = tamanyo(tupla);
		Tupla t (ssize);
		t.from_string(tupla);
		cout << "Operacion recibida: " << operacion << endl;
		cout << "Tupla recibida: " << tupla << endl;

		string message;

		// Si recibimos "END OF SERVICE" --> Fin de la comunicación
		if (0 == strcmp(buffer, MENS_FIN)) {
			out = true; // Salir del bucle
		} else {
			// Contamos las vocales recibidas en el mensaje anterior
		
			
			if (strcmp(operacion,"PN" )== 0){
				ML.PostNote(t);
				message = "OK";
			}
			else if (strcmp(operacion,"ReadN" )== 0){
				Tupla r (ssize);
				ML.ReadNote(t,r);
				message = r.to_string();
				cout << "La tupla que se devuelve al cliente queda así: " <<  message << endl;
			}
			else if (strcmp(operacion,"RN") == 0){
				Tupla r (ssize);
				ML.RemoveNote(t,r);
				message = r.to_string();
				cout << "La tupla que se devuelve al cliente queda así: " <<  message << endl;
			}
			// Enviamos la respuesta
		
			int send_bytes = soc.Send(client_fd, message);
			if(send_bytes == -1) {
				string mensError(strerror(errno));
    			cerr << "Error al enviar datos: " + mensError + "\n";
				// Cerramos los sockets
				soc.Close(client_fd);
				exit(1);
			}
		}
	}
	soc.Close(client_fd);
}
//-------------------------------------------------------------
int main(int argc, char* argv[]) {

 if(argc < 2)
	{
	    cerr << "Invocar como:" << endl;
	    cerr << "   servidorLinda <Port_LS>" << endl;
	    cerr << "      <Port_LS>: puerto del servidor de almacenamiento" << endl;
	    return 1;
	}
	const int N = 2;
	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = "localhost";
    int SERVER_PORT = atoi(argv[1]);
    thread cliente[N];
    int client_fd[N];


	MonitorLinda ML;

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

	for (int i=0; i<max_connections; i++) {
		// Accept
		client_fd[i] = socket.Accept();

		if(client_fd[i] == -1) {
			string mensError(strerror(errno));
    		cerr << "Error en el accept: " + mensError + "\n";
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}

		cout << "Lanzo thread nuevo cliente " + to_string(i) + "\n";
		cliente[i] = thread(&servCliente, ref(socket), client_fd[i], ref(ML));
		cout << "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
	}
	
	//¿Qué pasa si algún thread acaba inesperadamente?
	for (int i=0; i<max_connections; i++) {
		cliente[i].join();
	}

    // Cerramos el socket del servidor
    error_code = socket.Close(socket_fd);
    if (error_code == -1) {
		string mensError(strerror(errno));
    	cerr << "Error cerrando el socket del servidor: " + mensError + "\n";
	}

	// Despedida
	cout << "Bye bye" << endl;

    return error_code;
}
//-------------------------------------------------------------
