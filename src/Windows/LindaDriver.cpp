/*
 * --------------------------------------------------
 * -- Authors ---------------------------------------
 * -- aeri ------------------------------------------
 * -- ZgzInfinity -----------------------------------
 * --------------------------------------------------
 */

#include <iostream>
#include <iomanip>
#include <sstream>

#include "LindaDriver.hpp"

using namespace std;


#define DEFAULT_BUFLEN 512


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

LD::LD(string ip, string port)
{
	
    ConnectSocket = INVALID_SOCKET;

    const char *sendbuf = "PN:[XA]";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        //return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        //return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            //return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        //return 1;
    }
}

/*
 * Pre:la dimension de <<t>> es >=1 && <=6
 * Post: Ha informado al servidor correspondiente la tupla sobre la que debe ejcutar PostNote 
 */
void LD::PN(Tupla t)
{
	
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

    string message = "PN:" + t.to_string();
	
	
	// Send an initial buffer
    iResult = send( ConnectSocket, message.c_str(), (int)message.size(), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        //return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);
	
    // Receive until the peer closes the connection
	
    do {
		
		// Empty and clear the buffer
		memset(recvbuf, 0, DEFAULT_BUFLEN);

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
			cout << iResult << endl;
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());
	
		if (strcmp (recvbuf,"ACK") == 0){
			break;
		}

    } while( iResult > 0 );
	
	// Empty and clear the buffer
	memset(recvbuf, 0, DEFAULT_BUFLEN);
	
	        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
			cout << recvbuf << endl;
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    const char *ackbuf = "ACK";
    
    iResult = send( ConnectSocket, ackbuf, (int)strlen(ackbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        //return 1;
    }
};


Tupla LD::RN(Tupla t)
{
    string message = "RN:" + t.to_string();
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Send an initial buffer
    iResult = send( ConnectSocket, message.c_str(), (int)message.size(), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        //return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);
	
    // Receive until the peer closes the connection
	
    do {
		
		// Empty and clear the buffer
		memset(recvbuf, 0, DEFAULT_BUFLEN);

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
			cout << iResult << endl;
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());
	
		if (strcmp (recvbuf,"ACK") == 0){
			break;
		}

    } while( iResult > 0 );
	
	// Empty and clear the buffer
	memset(recvbuf, 0, DEFAULT_BUFLEN);
	
	        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
			cout << recvbuf << endl;
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    const char *ackbuf = "ACK";
    
    iResult = send( ConnectSocket, ackbuf, (int)strlen(ackbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        //return 1;
    }


	cout << "vuelta:" << recvbuf << endl;
    Tupla r(tamanyo(recvbuf));
    r.from_string(recvbuf);
    return r;
};

Tupla LD::ReadN(Tupla t)
{
    string message = "ReadN:" + t.to_string();
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Send an initial buffer
    iResult = send( ConnectSocket, message.c_str(), (int)message.size(), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        //return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);
	
    // Receive until the peer closes the connection
	
    do {
		
		// Empty and clear the buffer
		memset(recvbuf, 0, DEFAULT_BUFLEN);

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
			cout << iResult << endl;
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());
	
		if (strcmp (recvbuf,"ACK") == 0){
			break;
		}

    } while( iResult > 0 );
	
	// Empty and clear the buffer
	memset(recvbuf, 0, DEFAULT_BUFLEN);
	
	        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
			cout << recvbuf << endl;
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    const char *ackbuf = "ACK";
    
    iResult = send( ConnectSocket, ackbuf, (int)strlen(ackbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        //return 1;
    }


	cout << "vuelta:" << recvbuf << endl;
    Tupla r(tamanyo(recvbuf));
    r.from_string(recvbuf);
    return r;
};


void LD::STOP(){

    const char *stopbuf = "END OF SERVICE";
	
	
		iResult = send( ConnectSocket, stopbuf, (int)strlen(stopbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        //return 1;
    }
	
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();


}









