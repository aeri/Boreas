#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include "Socket.hpp"
#include "tuplas.hpp"

#include <mutex>

bool STOP = false;
const int N = 10;
const int MAX_ATTEMPS = 10;

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
    /********************************************
        CONEXIÓN DEL SERVIDOR LINDA CON LOS SERVIDORES
        DE ALMACENAMIENTO DE TUPLAS
        *********************************************/

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
    int length = 100;
    char buffer[length];

    string operation;

    string message;

    bool out = false;  // Inicialmente no salir del bucle
    while(!out)
	{
	    // Recibimos la operación que el cliente desea realizar
	    int rcv_bytes = soc.Recv(client_fd, operation, length);
	    if(rcv_bytes == -1)
		{
		    string mensError(strerror(errno));
		    cerr << "Error al recibir datos: " + mensError + "\n";
		    // Cerramos los sockets
		    soc.Close(client_fd);
		}

	    cout << "Mensaje recibido: " << operation << endl;
	    // Contestamos al cliente que su petición está dispuesta a ser procesada
	    int send_bytes = soc.Send(client_fd, "OK");

	    if(send_bytes == -1)
		{
		    cerr << "Error al enviar datos: " << strerror(errno) << endl;
		    // Cerramos el socket
		    soc.Close(client_fd);
		}

	    // Recibimos la operación que el cliente desea realizar
	    rcv_bytes = soc.Recv(client_fd, buffer, length);
	    if(rcv_bytes == -1)
		{
		    string mensError(strerror(errno));
		    cerr << "Error al recibir datos: " + mensError + "\n";
		    // Cerramos los sockets
		    soc.Close(client_fd);
		}

	    cout << "Tupla: " << buffer << endl;
	    // Si recibimos "END OF SERVICE" --> Fin de la comunicación
	    int ssize = tamanyo(buffer);
	    message = operation + ":" + buffer;
	    cout << "Mensaje a enviar: " << message << endl;


	    if (ssize <= 3){
		    cout << "<=3" << endl;
		    if(operation == "PN")
			{
				cout << "####POST NOTE####" << endl;
			    int send_bytes = server1.Send(socket_s1, message);
			    if(send_bytes == -1)
				{
				    cerr << "Error al enviar datos: " << strerror(errno) << endl;
				    // Cerramos el socket
				    server1.Close(socket_s1);
				}
				else{
					cout << "CORRECTO " << send_bytes << " bytes enviados." << endl;
				}
			    int read_bytes = server1.Recv(socket_s1, buffer, strlen(buffer));
			    if(read_bytes == -1)
				{
				    string mensError(strerror(errno));
				    cerr << "Error al recibir datos: " + mensError + "\n";
				    // Cerramos los sockets
				    server1.Close(socket_s1);
				}
			    send_bytes = soc.Send(client_fd, "OK");
			    cout << "             FIN PN" << endl;

			    if(send_bytes == -1)
				{
				    cerr << "Error al enviar datos: " << strerror(errno) << endl;
				    // Cerramos el socket
				    soc.Close(client_fd);
				}
			}

			else if(operation == "ReadN")
			{
				cout << "####READ NOTE####" << endl;
			    int send_bytes = server1.Send(socket_s1, message);
			    if(send_bytes == -1)
				{
				    cerr << "Error al enviar datos: " << strerror(errno) << endl;
				    // Cerramos el socket
				    server1.Close(socket_s1);
				}
				else{
					cout << "CORRECTO " << send_bytes << " bytes enviados." << endl;
				}
			    int read_bytes = server1.Recv(socket_s1, buffer, strlen(buffer));
			    if(read_bytes == -1)
				{
				    string mensError(strerror(errno));
				    cerr << "Error al recibir datos: " + mensError + "\n";
				    // Cerramos los sockets
				    server1.Close(socket_s1);
				}
			    send_bytes = soc.Send(client_fd, "OK");
			    cout << "             FIN ReadN" << endl;

			    if(send_bytes == -1)
				{
				    cerr << "Error al enviar datos: " << strerror(errno) << endl;
				    // Cerramos el socket
				    soc.Close(client_fd);
				}
			}
			else
				{
				cout << "####REMOVE NOTE####" << endl;
			    int send_bytes = server1.Send(socket_s1, message);
			    if(send_bytes == -1)
				{
				    cerr << "Error al enviar datos: " << strerror(errno) << endl;
				    // Cerramos el socket
				    server1.Close(socket_s1);
				}
				else{
					cout << "CORRECTO " << send_bytes << " bytes enviados." << endl;
				}
			    int read_bytes = server1.Recv(socket_s1, buffer, strlen(buffer));
			    if(read_bytes == -1)
				{
				    string mensError(strerror(errno));
				    cerr << "Error al recibir datos: " + mensError + "\n";
				    // Cerramos los sockets
				    server1.Close(socket_s1);
				}
			    send_bytes = soc.Send(client_fd, buffer);
			    cout << "             FIN RN" << endl;

			    if(send_bytes == -1)
				{
				    cerr << "Error al enviar datos: " << strerror(errno) << endl;
				    // Cerramos el socket
				    soc.Close(client_fd);
				}
			}
		}
		else if (ssize >3 && ssize <6){
		    cout << "4||5" << endl;
		    if(operation == "PN")
			{
			    int send_bytes = server2.Send(socket_s2, message);
			    if(send_bytes == -1)
				{
				    cerr << "Error al enviar datos: " << strerror(errno) << endl;
				    // Cerramos el socket
				    server2.Close(socket_s2);
				}
				else{
					cout << "CORRECTO " << send_bytes << " bytes enviados." << endl;
				}
			    int read_bytes = server2.Recv(socket_s2, buffer, strlen(buffer));
			    if(read_bytes == -1)
				{
				    string mensError(strerror(errno));
				    cerr << "Error al recibir datos: " + mensError + "\n";
				    // Cerramos los sockets
				    server1.Close(socket_s1);
				}
			    send_bytes = soc.Send(client_fd, "OK");
			    cout << "             FIN PN" << endl;

			    if(send_bytes == -1)
				{
				    cerr << "Error al enviar datos: " << strerror(errno) << endl;
				    // Cerramos el socket
				    soc.Close(client_fd);
				}
			}
		}
		else{
		    cout << "6" << endl;
		    if(operation == "PN")
			{
			    int send_bytes = server3.Send(socket_s3, message);
			    if(send_bytes == -1)
				{
				    cerr << "Error al enviar datos: " << strerror(errno) << endl;
				    // Cerramos el socket
				    server3.Close(socket_s3);
				}
				else{
					cout << "CORRECTO " << send_bytes << " bytes enviados." << endl;
				}
			    int read_bytes = server3.Recv(socket_s3, buffer, strlen(buffer));
			    if(read_bytes == -1)
				{
				    string mensError(strerror(errno));
				    cerr << "Error al recibir datos: " + mensError + "\n";
				    // Cerramos los sockets
				    server1.Close(socket_s1);
				}
			    send_bytes = soc.Send(client_fd, "OK");
			    cout << "             FIN PN" << endl;

			    if(send_bytes == -1)
				{
				    cerr << "Error al enviar datos: " << strerror(errno) << endl;
				    // Cerramos el socket
				    soc.Close(client_fd);
				}
			}

		}
	}
}
    soc.Close(client_fd);
}

int main(int argc, char* argv[])
{
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
    thread cliente[N];
    int client_fd[N];

    /********************************************
    UNA VEZ CONECTADOS LOS ALMACENES SE INICIA EL
    SERVIDOR LINDA
    *********************************************/

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

    // thread
    int contados = 0;
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
		    exit(1);
		}
	    if(!STOP)
		{
		    string aux = "Lanzo thread nuevo cliente " + to_string(i) + "\n";

		    cout << aux;

		    thread(&servCliente, ref(socket), client_fd[i], argv[2], atoi(argv[3]), argv[4], atoi(argv[5]),
		           argv[6], atoi(argv[7])).detach();
		    aux = "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
		    cout << aux;

		    contados++;
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