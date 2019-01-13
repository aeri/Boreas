#!/bin/bash

for i in $(seq 1 10)
do
	gnome-terminal -e "./clienteInteractivo localhost 30000"
done

./cargacl localhost 30000