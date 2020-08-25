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
 * Fichero de interfaz del monitor de almacenamiento de tuplas.
 */

#ifndef _MONITORLINDA_HPP_
#define _MONITORLINDA_HPP_

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include "Tuple.hpp"
#include "Socket.hpp"

using namespace std;

class MonitorLinda
{
   private:
      struct bbdd{ 
           //Tipo Matriz
           struct Nodo
           {
                string valor; //
                Nodo* sigComp; //Siguiente componente de la tupla leida (vertical)
                Nodo* sigTupla; //Siguiente tupla (horizontal)
            };
            Nodo* primero; //Primer elemento de la primera tupla
       };

       // Variables permanentes del monitor
   
       bbdd tupleSpace1;   //Matriz tuplas dimension 1
       bbdd tupleSpace2;   //Matriz tuplas dimension 2
       bbdd tupleSpace3;   //Matriz tuplas dimension 3
       bbdd tupleSpace4;   //Matriz tuplas dimension 4
       bbdd tupleSpace5;   //Matriz tuplas dimension 5
       bbdd tupleSpace6;   //Matriz tuplas dimension 6

       mutex mtxMonitor;  // FUNDAMENTAL: mutex
   
      // Variables condición del monitor

      condition_variable hay_tupla1; //indica si hay tuplas de dimension 1
      condition_variable hay_tupla2; //indica si hay tuplas de dimension 2
      condition_variable hay_tupla3; //indica si hay tuplas de dimension 3
      condition_variable hay_tupla4; //indica si hay tuplas de dimension 4
      condition_variable hay_tupla5; //indica si hay tuplas de dimension 5
      condition_variable hay_tupla6; //indica si hay tuplas de dimension 6
   
    public:
      // Colección de funciones del monitor
       MonitorLinda(); //constructor

       ~MonitorLinda(); // Destrutor

      void PostNote(Tuple t); //Añade una tupla al espacio de tuplas

      void RemoveNote(Tuple t, Tuple& r);  //Busca una tupla, toma su valor y la borra del espacio de tuplas

      void ReadNote(Tuple t, Tuple& r, bool locked); //Busca una tupla en el espacio de tuplas con un patrón específico
};
#endif
