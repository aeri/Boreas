

<p align="center">
  <img  src="https://i.imgur.com/FjXBIDE.png">
</p>


# Boreas
Programming of Concurrent and Distributed Systems Project:

C++ LINDA implementation
## Design
El objetivo del proyecto es desarrollar un sistema de coordinación Linda, distribuido. El sistema ofrecerá las operaciones: PN, RN y readN.
Por simplicidad, las tuplas serán planas, es decir, no existen tuplas donde uno de sus elementos sea a su vez otra tupla (tuplas anidadas), tendrán una longitud máxima de 6 elementos y todos los elementos serán de tipo string.

![alt text][logo]

[logo]: 
https://i.imgur.com/BNRPF3y.jpg "Linda space system"

La figura muestra una abstracción del sistema a desarrollar.
El servidor Linda publica la interfaz del sistema de coordinación distribuido. Esta interfaz ofrece a los procesos remotos cinco operaciones: conectarse al servicio y desconectarse de él, además de las tres operaciones de manipulación de tuplas. Un proceso C++ usará la librería Linda driver, que se implementa como parte de la solución, para invocar estas operaciones remotamente.


Por otro lado, el servidor Linda estará compuesto por tres servidores, posiblemente ejecutándose en diferentes máquinas, responsables de almacenar y gestionar las operaciones que involucran a las truplas que ellos almacenan. En concreto, el primer servidor trabajará con las tuplas de tamaño 1 a 3, el segundo con las de tamaño 4 y 5, mientras que el tercero se encargará de las tuplas de tamaño 6. Internamente, las tuplas se almacenarán en memoria.
