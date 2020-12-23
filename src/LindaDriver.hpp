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
 * Fichero de interfaz del modulo LindaDriver
 */

#ifndef _LINDADRIVER_HPP_
#define _LINDADRIVER_HPP_

#include <cstring>
#include <iostream>
#include "Socket.hpp"
#include "Tuple.hpp"
#include <AES/AES.hpp>


class LD : public Socket
{
   private:
    string ip; //dirección ip
    int socket_fd; //socket de conexión

   public:
    LD(string ip, string p); //constructor 
    void PN(Tuple t); //PostNote
    Tuple RN(Tuple t); //RemoveNote
    Tuple RD(Tuple t); //ReadNote
    Tuple RX(Tuple t, bool& found); //ReadNote
    void STOP();//cierra la conexión del socket
};

#endif
