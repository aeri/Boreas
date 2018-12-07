#ifndef _LINDADRIVER_HPP_
#define _LINDADRIVER_HPP_

#include <cstring>
#include <iostream>
#include "tuplas.hpp"
#include "Socket.hpp"

class LD : public Socket{
private:
	int port;
	string ip;
	int socket_fd;

public:

	LD (string ip, string p);
	void PN (Tupla t);
	Tupla RN (Tupla t);
	Tupla ReadN (Tupla t);

};


#endif