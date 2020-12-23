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

/*
 * Fichero de implementación del módulo LindaDriver
 */

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include "LindaDriver.hpp"
#include "Socket.hpp"
#include "Tuple.hpp"
#include <AES/AES.hpp>
//#include "base64/base64.h"

using namespace std;

string nf ("NOT_FOUND");

int string_size(char * str)
{
    int Size = 0;
    while (str[Size] != '\0') Size++;
    return Size;
}


/*
 * Pre: <<s>> es un string que representa una tupla cuyas componentes están sepradas por comas
 * Post: Devuelve el número de componentes separadas por comas que hay en <<s>>
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

/*
 * Pre: <<ip>> es una direccion ip valida y <<p>> es un puerto válido
 * Post: Constructor de la clase LD
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
	    cerr << "Connection failed!" << endl;
	}
};

int string_size(unsigned char * str)
{
    int Size = 0;
    while (str[Size] != '\0') Size++;
    return Size;
}


/*
 * Pre:la dimension de <<t>> es >=1 && <=6
 * Post: Ha informado al servidor correspondiente la tupla sobre la que debe ejcutar PostNote 
 */
void LD::PN(Tuple t)
{
    const int MESSAGE_SIZE = 4001;

    string message = "PN:" + t.to_string();

    //message.resize(128, '\0');

    unsigned char data[message.length() + 1];
    copy(message.data(), message.data() + message.length() + 1, data);
	//copy( message.begin(), message.end(), data );
	
	cout << data << endl;
	
	
	unsigned char key[] = { 0xf0, 0x8b, 0x82, 0xff, 0xee, 0xef, 0x76, 0x67, 0xfa, 0xaf, 0xfa, 0x0f, 0x5c, 0x7d, 0x8e, 0xbe }; //key example
	unsigned char iv[] =  { 0xfa, 0xfa, 0xfa, 0xfa, 0xbe, 0xba, 0xca, 0xfe, 0xee, 0xaa, 0x11, 0xb4, 0xc7, 0xe1, 0xf1, 0xaa };
	unsigned int plainLen = string_size(data) * sizeof(unsigned char);  //bytes in plaintext
	unsigned int outLen = 0;  // out param - bytes in сiphertext

	AES aes(128);  //128 - key length, can be 128, 192 or 256
	unsigned char* c = aes.EncryptCBC(data, plainLen + 1, key, iv, outLen);

	//cout << "Dec bytes str: " << message.size() << endl;
	//cout << "Dec bytes: " << plainLen << endl;
	//cout << "Cod bytes: " << outLen << endl;

    int send_bytes = Send(socket_fd, c, outLen);



    if(send_bytes == -1)
	{
	    cerr << "Error at PN SEND: " << strerror(errno) << endl;
	    // Cerramos el socket
	    Close(socket_fd);
	    exit(1);
	}
    // Buffer para almacenar la respuesta, como char[]
    string buffer;

	int read_bytes = Recv(socket_fd, buffer, MESSAGE_SIZE);

	if (read_bytes <= 0){
		cerr << "Error at PN ACK" << endl;
	}
};


/*
 * Pre:la dimension de <<t>> es >=1 && <=6
 * Post: Ha informado al servidor correspondiente la tupla sobre la que debe ejcutar RemoveNote y la devuelve 
 */
Tuple LD::RN(Tuple t)
{
    const int MESSAGE_SIZE = 4001;

    string message = "RN:" + t.to_string();

    int send_bytes = Send(socket_fd, message);

    if(send_bytes == -1)
	{
	    cerr << "Error at RN SEND: " << strerror(errno) << endl;
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
	    cerr << "Error at RN RESPONSE: " + mensError + "\n";
	    // Cerramos los sockets
	    Close(socket_fd);
	}
    Tuple r(tamanyo(buffer));
    r.from_string(buffer);
    return r;
};


/*
 * Pre:la dimension de <<t>> es >=1 && <=6
 * Post:Ha informado al servidor correspondiente la tupla sobre la que debe ejcutar ReadNote y la devuelve 
 */
Tuple LD::RD(Tuple t)
{
    const int MESSAGE_SIZE = 4001;

    string message = "RD:" + t.to_string();

    int send_bytes = Send(socket_fd, message);

    if(send_bytes == -1)
	{
	    cerr << "Error at RD SEND: " << strerror(errno) << endl;
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
	    cerr << "Error at RD RESPONSE: " + mensError + "\n";
	    // Cerramos los sockets
	    Close(socket_fd);
	}
    Tuple r(tamanyo(buffer));
    r.from_string(buffer);
    return r;
};

Tuple LD::RX(Tuple t, bool& found)
{
    const int MESSAGE_SIZE = 4001;

    string message = "RX:" + t.to_string();

    int send_bytes = Send(socket_fd, message);

    if(send_bytes == -1)
	{
	    cerr << "Error at RD SEND: " << strerror(errno) << endl;
	    // Cerramos el socket
	    Close(socket_fd);
	    exit(1);
	}

    // Buffer para almacenar la respuesta, como char[]
    string buffer;
    int read_bytes = Recv(socket_fd, buffer, MESSAGE_SIZE);

    Tuple r(tamanyo(buffer));

    if(read_bytes == -1)
	{
	    string mensError(strerror(errno));
	    cerr << "Error at RD RESPONSE: " + mensError + "\n";
	    // Cerramos los sockets
	    Close(socket_fd);
	}


	if (buffer.compare(nf) == 0){
		found = false;
		r.set(1, nf);

	}
	else{
		found = true;
	    r.from_string(buffer);
	}

    return r;
};

/*
 * Pre: ---
 * Post:Cierra la conexión con el cliente enviando mensaje END OF SERVICE
 */
void LD::STOP()
{
    Send(socket_fd, "END OF SERVICE");
    Close(socket_fd);
};
