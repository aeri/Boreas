#ifndef _TUPLAS_HPP_
#define _TUPLAS_HPP_

#include <cstring>
#include <iostream>
#include "tuplas.hpp"
#include "Socket.hpp"

class LD{
private:
	int port;
	string ip;
	Socket socket;
	int socket_fd;

public:

	LD (string p, string ip);
	void PN (Tupla t);
	Tupla RN (Tupla t);
	Tupla ReadN (Tupla t);

}


#endif