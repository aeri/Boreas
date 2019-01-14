#!/bin/bash
#compila y ejecuta el sistema con el mismo numero de clientes interactivos y mixtos 

make clean

make

if [ $# -ne 5 ]
then
    echo "Numero incorrecto de parametros"
    echo " Se necesitan 4 parametros para ejecutar"
    echo " Puerto ServidorLinda "
    echo " Puerto Servei 1 "
    echo " Puerto Servei 2 "
    echo " Puerto Servei 3 "
    echo " Puerto Cliente "
    exit 1
fi  

gnome-terminal -e "./servidorLinda $1 localhost $2 localhost $3 localhost $4"

gnome-terminal -e "./servei $2"
gnome-terminal -e "./servei $3"
gnome-terminal -e "./servei $4"


# Lanzamiento de clientes interactivos
for i in $(seq 1 $5)
do
        gnome-terminal -e "./clienteInteractivo localhost $1"
done

# Lanzamiento de clientes mixtos
for i in $(seq 1 $5)
do
        gnome-terminal -e "./clienteMixto localhost $1"
done
