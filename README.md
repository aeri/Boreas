<p align="center">
    <img src="doc/logo.png" alt="Logo" width=100 height=100>
  </a>

  <h3 align="center">Boreas</h3>

  <p align="center">
  A <b>cute</b> Linda implementation in C++
    <br>
    <a href="https://github.com/aeri/Boreas/blob/master/README.md#Getting-Started">Getting Started</a>
    ·
    <a href="https://github.com/aeri/Boreas/blob/master/README.md#deployment">Deployment</a>
  </p>
</p>

---
A lightweight proof of concept of the **Linda** coordination system in C++ using TCP sockets, to be used in programming concurrent and distributed systems.
The system implements the following operations:

* **OUT**: Insert a tuple, writing it into tuplespace.
* **IN**: Read and remove (consume) a tuple from tuplespace. 
* **RD**: Non-destructively retrieve a tuple from tuplespace.
* **RDP**: Non-destructively retrieve a tuplespace without blocking.

The Boreas LindaServer is a standalone binary that publishes the interface of coordination system, handles client requests and manages the tuples in memory. This interface offers the remote processes five operations: connect to and disconnect from the service, in addition to the four tuple manipulation operations.

A C++ process can use the [LindaDriver](lib/LindaDriver/) library, which is implemented as part of the solution, to invoke these operations remotely.

## Getting Started

This section shows how to build and use the server for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

* To compile the Boreas environment you need an operating system that supports POSIX Threads. Unix-like POSIX-conformant operating systems such as GNU/Linux, FreeBSD, NetBSD, OpenBSD, macOS...

* Make (build automation tool)

* gcc version >= 4.8.5 with POSIX thread model


### Build
To build the server and the examples it is only necessary to use the Makefile from project root. 

```
$> make
```

And all files will be compiled consecutively:

```
g++ -c -I. -Ilib/Socket -O2 -std=c++11 -lsockets -Wall lib/Socket/Socket.cpp -o lib/Socket/Socket.o
g++ -c -I. -Ilib/Socket -O2 -std=c++11 -lsockets -Wall src/server/lindaServer.cpp -o src/server/lindaServer.o
g++ -c -I. -Ilib/Socket -O2 -std=c++11 -lsockets -Wall src/examples/example.cpp -o src/examples/example.o
........
```

### Deployment

Once the build process is finished, the binaries will have been generated in the `bin/` folder and the server can be started as follows: 

```
./lindaServer <Port_LS>
```
><Port_LS>: Linda server port

In addition, two examples are provided in order to test and shows how a client could works:

* **./example** Basic example of the use of LindaDriver, performs some operations with tuples.
* **./interactiveExample** Client in which operations can be performed in the tuple space interactively through guided on-screen instructions.

#### Windows Support

Currently, support for Windows systems is limited and only the driver is available to act as a client, therefore it is necessary that the server structure is deployed on Unix-like systems. The driver allows to perform all the operations available by the server, however this driver has not been fully tested and may present some operational problems.

The driver needs to be linked with the ```-lws2_32``` option if MinGW is used.

### Protocol

Once the server is started any network client can perform Linda operations on the tuple space. To do this, a TCP frame must be sent with data in the following format:

```
<OPERATION>:<TUPLE>
```
>**OPERATION**: Operation must contain the operation to be performed in the space of tuples, 
it can be `OUT`, `IN`, `RD` and `RDP`

>**TUPLE**: It must contain the components of the tuple separated by commas and enclosed in square brackets. Wildcards can be used for tuple components in RN and RD/RX operations. For simplicity, a wildcard variable will be indicated by a string composed of "?" and a capital letter between "A" and "Z".

> For simplicity, the tuples are flat, that is to say, there are no tuples where one of their elements is another tuple (nested tuples), they will have a maximum length of 6 elements and all the elements will be of string type.

For example, the **OUT** operation with the *{"1","username","token"}* tuple would be coded as follows:

``` {"
OUT:[1,username,token]
```
This is represented in HEX as `4f55543a5b312c757365726e616d652c746f6b656e5d`

The server will be able to send the following responses:

* In case the operation is a tuple insertion, the server will immediately respond with `OK`
* In case the operation is a non-blocking tuple read operation and the tuple does not exist, `NOT_FOUND` will be returned.
* In case the operation is a blocking operation, a response will not be provided until the fetch condition is satisfied. The tuple will be returned in the format of `<TUPLE>`

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](COPYING) file for details
