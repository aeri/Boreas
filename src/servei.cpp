/*
 * ----------------------------------------------------------
 * -- Programación de sistemas concurrentes y distribuidos --
 * -- Trabajo práctico : Servidor Linda ---------------------
 * -- Autores y NIP -----------------------------------------
 * -- Daniel Naval Alcalá  739274 ---------------------------
 * -- Alejandro Omist Casado 737791 -------------------------
 * -- Rubén Rodríguez Esteban 737215 ------------------------
 * -- José Manuel Romero Clavería 740914 --------------------
 * ----------------------------------------------------------
 */

/*
 * Fichero de implementación de los servidores de almacenamiento de tuplas
 */

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

/*
 * Pre: <<s>> es una cadena de caracteres
 * Post: Devuelve cuantas subcadenas de caracteres hay separadas por comas
 */
int tamanyo(string s)
{
    // Flujo de datos asociado a string
    stringstream ss(s);
    int dimension = 1;
    for(unsigned int i = 0; i < s.size(); i++)
    {
	// Si se lee el caracter delimitador
        if(ss.get() == ',')
        {
	    // Incremento de las componentes
            ++dimension;
        }
    }
    return dimension;
}

/*
 * Pre: Es un thread que tiene los siguientes parámetros:
 * 	<<soc>> es un socket definido con un puerto y una ip espeficias
 *      que permite el envio y la recepción de mensajes a los clientes,
 *      <<client_fd>> es el descriptor asociado al socket y <<ML>> es el
 *      monitor gestor de operaciones de los servidores.
 * Post: El thread permite gestionar al servidor de Linda la petición del cliente
 *       de forma que una vez concluida le envia la respuesta
 */
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

		// Si recibimos "END OF SERVICE" --> Fin de la comunicación
		if (0 == strcmp(buffer, MENS_FIN)) {
			out = true; // Salir del bucle
		} 
		else {
			// Recibe el mensaje como operacion:tupla
			// Separa en <<operacion>> la operacion (PN, ReadN o RN) y 
			// en <<tupla>> la tupla con la que trabajar
			char* operacion = strtok (buffer,":");
			char* tupla = strtok (NULL, ":");
			int ssize = tamanyo(tupla);
			Tupla t (ssize);
			t.from_string(tupla);
			cout << "Operacion recibida: " << operacion << endl;
			cout << "Tupla recibida: " << tupla << endl;

			string message;
			// Determinación de la operación
			if (strcmp(operacion,"PN" )== 0){
				// La operacion es PostNote
				// Ejecución de la operación del monitor
				ML.PostNote(t);
				message = "OK";
			}
			else if (strcmp(operacion,"ReadN" )== 0){
				// La operacion es ReadNote
				Tupla r (ssize);
				// Ejecución de la operación del monitor
				ML.ReadNote(t,r);
				message = r.to_string();
				cout << "La tupla que se devuelve al cliente queda así: " <<  message << endl;
			}
			else if (strcmp(operacion,"RN") == 0){
				// La operación es RemoveNote
				Tupla r (ssize);
				// Ejecución de la operación del monitor
				ML.RemoveNote(t,r);
				message = r.to_string();
				cout << "La tupla que se devuelve al cliente queda así: " <<  message << endl;
			}
			// se guarda en <<message>> la respuesta al cliente
			// Si es Postnote se envia OK
			// Si es ReadNote o RemoveNote se envia la tupla pedida
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
	// Cerramos cliente
	cout << "Se cierra un thread de cliente" << endl;
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
	const int N = 100;
	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = "localhost";
    int SERVER_PORT = atoi(argv[1]);
    thread cliente[N];
    int client_fd[N];

	// Creación del monitor para guardar las tuplas en los distintos servidores
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
		// Lanzamiento del thread con la petición del cliente
		thread(&servCliente, ref(socket), client_fd[i], ref(ML)).detach();
		cout << "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
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
