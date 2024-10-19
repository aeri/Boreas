#ifndef _LINDADRIVER_HPP_
#define _LINDADRIVER_HPP_

#include "Socket.hpp"
#include "Tuple.hpp"
#include <cstring>
#include <iostream>

class LD : public Socket {
private:
  string ip;     // dirección ip
  int socket_fd; // socket de conexión

public:
  LD(string ip, string p);        // constructor
  void PN(Tuple t);               // PostNote
  Tuple RN(Tuple t);              // RemoveNote
  Tuple RD(Tuple t);              // ReadNote
  Tuple RX(Tuple t, bool &found); // ReadNote
  void STOP();                    // cierra la conexión del socket
};

#endif
