#!/bin/bash

#----------------------------------
# Autor: Rubén Rodríguez Esteban
# NIP: 737215
#----------------------------------

# Comprobación del número de parámetros del script
if [ $# -ne 3 ]
then
    echo "Numero incorrecto de parametros"
    echo " Se necesitan 3 parametros para ejecutar"
    echo " N clientes test "
    echo " Direccion IP de servidor Linda "
    echo " Puerto de escucha de servidor Linda"
    exit 1
fi

numTuplas=$((1000/$1))

# Para asegurar que el número mínimo de tuplas que cargue
# cada servidor sea procesable por la lectura
# se ajusta el valor para conseguirlo
while [ $numTuplas -lt 100 ]
do
	numTuplas=$(($numTuplas+10))
done
echo $numTuplas

# Lanzamiento de clientes test
for i in $(seq 1 $1)
do
        gnome-terminal -e "./test $2 $3 $numTuplas"
done
