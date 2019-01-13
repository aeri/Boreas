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

# Lanzamiento de diez clientes interactivos
for i in $(seq 1 10)
do
	gnome-terminal -e "./clienteInteractivo localhost 30000"
done

