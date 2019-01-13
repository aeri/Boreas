#
# ----------------------------------------------------------
# -- Programación de sistemas concurrentes y distribuidos --
# -- Trabajo práctico de Servidor Linda -------------------- 
# -- Autores -----------------------------------------------
# -- Daniel Naval Alcalá 739274 ----------------------------
# -- Alejandro Omist Casado 739971 -------------------------
# -- Rubén Rodríguez Esteban 737215 ------------------------
# -- José Manuel Romero Clavería 740914 --------------------
# ----------------------------------------------------------
#

#!/bin/bash


# Comprobación del número de parámetros del script
if [ $# -ne 4 ]
then
    echo "Numero incorrecto de parametros"
    echo " Se necesitan 4 parametros para ejecutar"
    echo " N clientes interactivos "
    echo " M clientes mixtos "
    echo " Direccion IP de servidor Linda "
    echo " Puerto de escucha de servidor Linda"
    exit 1
fi

# Lanzamiento de clientes interactivos
for i in $(seq 1 $1)
do
        gnome-terminal -e "./clienteInteractivo $3 $4"
done

# Lanzamiento de clientes mixtos
for i in $(seq 1 $2)
do
        gnome-terminal -e "./clienteMixto $3 $4"
done
