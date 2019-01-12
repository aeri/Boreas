#!/bin/bash

for i in $(seq 1 10)
do
	gnome-terminal -e "./clienteInteractivo 10.3.17.86 30000"
done
