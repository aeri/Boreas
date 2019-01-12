/*
 * ----------------------------------------------------------
 * -- Programación de sistemas concurrentes y distribuidos --
 * -- Trabajo práctico : Servidor Linda ---------------------
 * -- Autores y NIP -----------------------------------------
 * -- Daniel Naval Alcalá  739274 ---------------------------
 * -- Alejandro Omist Casado 737791 -------------------------
 * -- Rubén Rodríguez Esteban 737215 ------------------------
 * -- José Manuel Romero Clavería 740914 --------------------
 * ----------------------------------------------------------
 */

/*
 * Fichero de interfaz del modulo LindaDriver
 */

#ifndef _LINDADRIVER_HPP_
#define _LINDADRIVER_HPP_

#include <cstring>
#include <iostream>
#include "Socket.hpp"
#include "tuplas.hpp"

class LD : public Socket
{
   private:
    int port; //puerto de conexión 
    string ip; //dirección ip
    int socket_fd; //socket de conexión

   public:
    LD(string ip, string p); //constructor 
    void PN(Tupla t); //PostNote
    Tupla RN(Tupla t); //RemoveNote
    Tupla ReadN(Tupla t); //ReadNote
    void STOP();//cierra la conexión del socket
};

#endif
