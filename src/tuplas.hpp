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
 * Fichero de interfaz del módulo Tupla.
 */

#ifndef _TUPLAS_HPP_
#define _TUPLAS_HPP_

#include <cstring>
#include <iostream>

using namespace std;

class Tupla
{
   private:
    int dimension;         // Dimensión de la tupla
    string vector[6];      // Estructura estática para almacenar las componentes de la tupla

   public:
    // Colección de funciones de la clase especificadas en 
    // el fichero de implementación del módulo tuplas
    Tupla(string p1);
    Tupla(string p1, string p2);
    Tupla(string p1, string p2, string p3);
    Tupla(string p1, string p2, string p3, string p4);
    Tupla(string p1, string p2, string p3, string p4, string p5);
    Tupla(string p1, string p2, string p3, string p4, string p5, string p6);
    Tupla(int dimension);
    void from_string(string tupla);
    string to_string();
    string get(int indice);
    void set(int indice, string campo);
    int size();
};

#endif
