/*
 * --------------------------------------------------
 * -- Authors ---------------------------------------
 * -- aeri ------------------------------------------
 * -- ZgzInfinity -----------------------------------
 * --------------------------------------------------
 */

/*
 * Fichero de interfaz del modulo LindaDriver
 */

#ifndef _LINDADRIVER_HPP_
#define _LINDADRIVER_HPP_

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include "tuplas.hpp"

using namespace std;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")#define WIN32_LEAN_AND_MEAN



class LD
{
private:
    int port; //puerto de conexión 
    string ip; //dirección ip
    SOCKET ConnectSocket; //socket de conexión
	WSADATA wsaData;
	struct addrinfo *result = NULL,
				*ptr = NULL,
				hints;
	
	int iResult;
	
	//int initWinsock();

   public:
    LD(string ip, string port); //constructor 
    void PN(Tupla t); //PostNote
    Tupla RN(Tupla t); //RemoveNote
    Tupla ReadN(Tupla t); //ReadNote
    void STOP();//cierra la conexión del socket
};

#endif
