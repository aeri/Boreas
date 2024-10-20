#ifndef _LINDADRIVER_HPP_
#define _LINDADRIVER_HPP_

#include "../Socket/Socket.hpp"
#include "../Tuple/Tuple.hpp"

class LD : public Socket {
private:
  string ip;     // dirección ip
  int socket_fd; // socket de conexión

public:
  LD(string ip, string p);        // constructor
  void OUT(Tuple t);               // PostNote
  Tuple IN(Tuple t);              // RemoveNote
  Tuple RD(Tuple t);              // ReadNote
  Tuple RDP(Tuple t, bool &found); // ReadNote
  void STOP();                    // cierra la conexión del socket
};

#endif
