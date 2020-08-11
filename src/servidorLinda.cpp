#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include "Socket.hpp"
#include "tuplas.hpp"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>



#include <mutex>

bool STOP = false;
const int N = 100;
const int MAX_ATTEMPS = 50;
const char MENS_FIN[] = "END OF SERVICE";


void capturarSIGINT(int s){
	cout << endl << "Señal " << s << " capturada." << endl;
	STOP = true;
}

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

void servCliente(Socket& soc, int client_fd, string ip1, int p1, string ip2, int p2, string ip3, int p3)
{

    int count = 0;
    int socket_s1;
    int socket_s2;
    int socket_s3;

    Socket server1(ip1, p1);
    Socket server2(ip2, p2);
    Socket server3(ip3, p3);

    do
	{
	    // Conexión con el servidor
	    socket_s1 = server1.Connect();
	    socket_s2 = server2.Connect();
	    socket_s3 = server3.Connect();
	    count++;

	    // Si error --> esperamos 1 segundo para reconectar
	    if(socket_s1 == -1 || socket_s2 == -1 || socket_s3 == -1)
		{
		    this_thread::sleep_for(chrono::seconds(1));
		}
	}
    while((socket_s1 == -1 || socket_s2 == -1 || socket_s3 == -1) && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_s1 == -1 || socket_s2 == -1 || socket_s3 == -1)
	{
	    cerr << "Error al conectar" << endl;
	}
    else
	{
	    cout << "Servidores conectados" << endl;

	    // Buffer para recibir el mensaje
	    // char MENS_FIN[] = "END OF SERVICE";
	    // Buffer para recibir el mensaje
	    int length = 200;
	    char message[length];
	    char buffer[length];

	    string operation;

	    bool out = false;  // Inicialmente no salir del bucle

	    int descriptor;

	    Socket serverX = server1;

	    while(!out)
		{
		    // Recibimos la operación y tupla que el cliente desea realizar
		    int rcv_bytes = soc.Recv(client_fd, message, length);
		    if(rcv_bytes <= 0 || rcv_bytes >= length )
			{
			    string mensError(strerror(errno));
			    cerr << "Error al recibir datos: " + mensError + "\n";
			    // Cerramos los sockets
			    soc.Close(client_fd);
			    out = true;
			    break;
			}

			message[rcv_bytes] = '\0';

			cout << rcv_bytes << endl;

		    cout << "Mensaje recibido: " << message << endl;

		    if(strncmp(message,MENS_FIN, length) == 0)
			{
				cout << "Salida detectada" << endl;
			    out = true;  // Salir del bucle
			}
		    else
			{
			    strncpy(buffer,message,length);
				
				char* operacion = strtok (message,":");
				char* tupla = strtok (NULL, ":");
				int ssize = tamanyo(tupla);
				Tupla t (ssize);
				t.from_string(tupla);
			    // Si recibimos "END OF SERVICE" --> Fin de la comunicación
			    cout << "Operación recibida: " << operacion << endl;

			    if(ssize <= 3)
				{
				    serverX = server1;
				    descriptor = socket_s1;
				}
			    else if(ssize > 3 && ssize < 6)
				{
				    serverX = server2;
				    descriptor = socket_s2;
				}
			    else if(ssize == 6)
				{
				    serverX = server3;
				    descriptor = socket_s3;
				}
			    else
				{
				    descriptor = -1;
				    cerr << "Fail to decode tuple size" << endl;
				    out = true;  // Salir del bucle
				    break;
				}

			    // Elegido servidor
			    int send_bytes = serverX.Send(descriptor, buffer);
			    if(send_bytes <= 0)
				{
				    cerr << "Error al enviar datos: " << strerror(errno) << endl;
				    // Cerramos el socket
				    serverX.Close(descriptor);
				    out = true;
				    break;
				}

			    int read_bytes = serverX.Recv(descriptor, buffer, length);

			    if(read_bytes <= 0)
				{
				    string mensError(strerror(errno));
				    cerr << "Error al recibir datos: " + mensError + "\n";
				    // Cerramos los sockets
				    serverX.Close(descriptor);
				    out = true;
				    break;
				}
				cout << "RESPONSE: " << buffer << endl;
			    send_bytes = soc.Send(client_fd, buffer);

			    if(send_bytes == -1)
				{
				    cerr << "Error al enviar datos: " << strerror(errno) << endl;
				    // Cerramos el socket
				    soc.Close(client_fd);
				    out = true;  // Salir del bucle
				}
			}
		}
	}
	cout << "Cerrado un servicio" << endl;
    server1.Send(socket_s1, MENS_FIN);
    server2.Send(socket_s2, MENS_FIN);
    server3.Send(socket_s3, MENS_FIN);

    int error_code1 = server1.Close(socket_s1);
    int error_code2 = server2.Close(socket_s2);
    int error_code3 = server3.Close(socket_s3);
    if(error_code1 == -1 || error_code2 == -1 || error_code3 == -1)
	{
	    cerr << "Error cerrando el socket: " << strerror(errno) << endl;
	}

    soc.Close(client_fd);
}

int main(int argc, char* argv[])
{

	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = capturarSIGINT;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

    if(argc < 8)
	{
	    cerr << "Invocar como:" << endl;
	    cerr << "   servidorLinda <Port_LS> <IP_S1> <Port_S1> <IP_S2> <Port_S2> <IP_S3> <Port_S3>" << endl;
	    cerr << "      <Port_LS>: puerto del servidor Linda" << endl;
	    cerr << "      <IP_S1>: IP del servidor 1" << endl;
	    cerr << "      <Port_S1>: puerto del servidor 1" << endl;
	    cerr << "      <IP_S2>: IP del servidor 2" << endl;
	    cerr << "      <Port_S2>: puerto del servidor 2" << endl;
	    cerr << "      <IP_S3>: IP del servidor 3" << endl;
	    cerr << "      <Port_S3>: puerto del servidor 3" << endl;
	    return 1;
	}

    string SERVER_ADDRESS = "localhost";
    int SERVER_PORT = atoi(argv[1]);
    int client_fd[N];


    cout << "Iniciando servidor Linda en puerto " << SERVER_PORT << endl;

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket socket(SERVER_PORT);

    // Bind
    int socket_fd = socket.Bind();
    if(socket_fd == -1)
	{
	    string mensError(strerror(errno));
	    cerr << "Error en el bind: " + mensError + "\n";
	    exit(1);
	}

    // Listen
    int max_connections = N;
    int error_code = socket.Listen(max_connections);
    if(error_code == -1)
	{
	    string mensError(strerror(errno));
	    cerr << "Error en el listen: " + mensError + "\n";
	    // Cerramos el socket
	    socket.Close(socket_fd);
	    exit(1);
	}

    // threads
    int i = 0;

    // Atendemos a las peticiones de los clientes
    while(i < max_connections && !STOP)
	{
	    // Accept
	    client_fd[i] = socket.Accept();

	    if(client_fd[i] == -1)
		{
		    string mensError(strerror(errno));
		    cerr << "Error en el accept: " + mensError + "\n";
		    // Cerramos el socket
		    socket.Close(socket_fd);
		    
		    for (int j = 0; j<i; ++j){
		    	cout << "Cerrando socket " << j << endl;
		    	socket.Close(client_fd[j]);
		    }
		    
		    exit(1);
		}
	    if(i < max_connections && !STOP)
		{
		    string aux = "Lanzo thread nuevo cliente " + to_string(i) + "\n";

		    cout << aux;

		    thread(&servCliente, ref(socket), client_fd[i], argv[2], atoi(argv[3]), argv[4], atoi(argv[5]),
		           argv[6], atoi(argv[7])).detach();
		    aux = "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
		    cout << aux;

		    i++;
		}
	    else
		{
		    string aux = "Se va a Finalizar el programa\n";
		    cout << aux;
		}
	}
    // join();
    return 0;
}
