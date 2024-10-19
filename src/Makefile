CC=g++

DRIVER=LindaDriver
MONITOR=monitorLinda
SOCKET_DIR=Socket
SOCKET=${SOCKET_DIR}/Socket

BASIC_EXAMPLE=example
INTERACTIVE_EXAMPLE=interactiveExample

SERVER=lindaServer
TUPLAS=Tuple


CPPFLAGS=-I. -I${SOCKET_DIR} -O2 -std=c++11 -lsockets -Wall# Flags compilacion
LDFLAGS=-pthread # Flags linkado threads

all: ${BASIC_EXAMPLE} ${SERVER} ${INTERACTIVE_EXAMPLE}
#----------------------------------------------------------------------------
#Para gestionar opciones de compilación según la máquina: hendrix tiene sus manías
#Descomentar la siguiente línea para compilar en hendrix
#SOCKETSFLAGS=-lsocket -lnsl
#-----------------------------------------------------------
# MONITOR
# Compilacion
${MONITOR}.o: ${MONITOR}.hpp ${MONITOR}.cpp
	${CC} -c ${CPPFLAGS} ${MONITOR}.cpp

# Linkado
${MONITOR}: ${TUPLAS}.o ${MONITOR}.o  
	${CC} ${TUPLAS}.o ${MONITOR}.o -o ${MONITOR} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# DRIVER
# Compilacion
${DRIVER}.o: ${DRIVER}.hpp ${DRIVER}.cpp
	${CC} -c ${CPPFLAGS} ${DRIVER}.cpp

# Linkado
${DRIVER}: ${TUPLAS}.o ${SOCKET}.o ${DRIVER}.o  
	${CC} ${LDFLAGS} ${TUPLAS}.o ${SOCKET}.o ${DRIVER}.o -o ${DRIVER} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# CLIENTE
# Compilacion
${BASIC_EXAMPLE}.o: ${BASIC_EXAMPLE}.cpp
	${CC} -c ${CPPFLAGS} ${BASIC_EXAMPLE}.cpp

# Linkado
${BASIC_EXAMPLE}: ${DRIVER}.o ${TUPLAS}.o ${SOCKET}.o ${BASIC_EXAMPLE}.o  
	${CC} ${LDFLAGS} ${DRIVER}.o ${TUPLAS}.o ${SOCKET}.o ${BASIC_EXAMPLE}.o -o ${BASIC_EXAMPLE} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# OTROCLIENTE
# Compilacion
${INTERACTIVE_EXAMPLE}.o: ${INTERACTIVE_EXAMPLE}.cpp
	${CC} -c ${CPPFLAGS} ${INTERACTIVE_EXAMPLE}.cpp

# Linkado
${INTERACTIVE_EXAMPLE}: ${DRIVER}.o ${TUPLAS}.o ${SOCKET}.o ${INTERACTIVE_EXAMPLE}.o  
	${CC} ${LDFLAGS} ${DRIVER}.o ${TUPLAS}.o ${SOCKET}.o ${INTERACTIVE_EXAMPLE}.o -o ${INTERACTIVE_EXAMPLE} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# SERVIDOR
# Compilacion
${SERVER}.o: ${SERVER}.cpp 
	${CC} -c ${CPPFLAGS} ${SERVER}.cpp

# Linkado
${SERVER}: ${TUPLAS}.o ${MONITOR}.o ${SOCKET}.o ${SERVER}.o
	${CC} ${LDFLAGS} ${TUPLAS}.o ${MONITOR}.o ${SOCKET}.o ${SERVER}.o -o ${SERVER} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# SOCKETS
# Compilacion libreria de Sockets
${SOCKET}.o: ${SOCKET}.hpp ${SOCKET}.cpp
	${CC} -c ${CPPFLAGS} ${SOCKET}.cpp -o ${SOCKET}.o
#-----------------------------------------------------------
# TUPLAS
# Compilacion
${TUPLAS}.o: ${TUPLAS}.hpp  ${TUPLAS}.cpp 
	${CC} -c ${CPPFLAGS} ${TUPLAS}.cpp -o ${TUPLAS}.o
#-----------------------------------------------------------		
# LIMPIEZA
clean:
	$(RM) ${SOCKET}.o
	$(RM) ${TUPLAS}.o
	$(RM) ${BASIC_EXAMPLE} ${BASIC_EXAMPLE}.o
	$(RM) ${INTERACTIVE_EXAMPLE} ${INTERACTIVE_EXAMPLE}.o
	$(RM) ${SERVER} ${SERVER}.o
	$(RM) ${DRIVER} ${DRIVER}.o
	$(RM) ${MONITOR} ${MONITOR}.o
