#ifndef _TUPLAS_HPP_
#define _TUPLAS_HPP_

#include <cstring>
#include <iostream>
#include "tuplas.hpp"

class LD{
private:
	string port;
	string ip;

public:

	LD (string p, string ip);
	void PN (Tupla t);
	Tupla RN (Tupla t);
	Tupla ReadN (Tupla t);

}


#endif