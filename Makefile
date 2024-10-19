CC=g++

LIBS_DIR = lib
SRC_DIR = src
EXAMPLES_DIR   = $(SRC_DIR)/examples
SERVER_DIR = $(SRC_DIR)/server


# Lib
DRIVER_DIR	= $(LIBS_DIR)/LindaDriver
MONITOR_DIR	= $(LIBS_DIR)/MonitorLinda
SOCKET_DIR	= $(LIBS_DIR)/Socket
TUPLE_DIR	= $(LIBS_DIR)/Tuple

DRIVER  = ${DRIVER_DIR}/LindaDriver
MONITOR	= ${MONITOR_DIR}/monitorLinda
SOCKET	= ${SOCKET_DIR}/Socket
TUPLE	= ${TUPLE_DIR}/Tuple

# Servidores
SERVER = ${SERVER_DIR}/lindaServer

# Clientes
BASIC_EXAMPLE   = ${EXAMPLES_DIR}/example
INTERACTIVE_EXAMPLE = ${EXAMPLES_DIR}/interactiveExample


CPPFLAGS=-I. -I${SOCKET_DIR} -O2 -std=c++11 -lsockets -Wall# Flags compilacion
LDFLAGS=-pthread # Flags linkado threads

all: ${SERVER} ${BASIC_EXAMPLE} ${INTERACTIVE_EXAMPLE}
#----------------------------------------------------------------------------
#SunOS Binary Compatibility
#SOCKETSFLAGS=-lsocket -lnsl
#-----------------------------------------------------------
# MONITOR
# Compilacion
${MONITOR}.o: ${MONITOR}.hpp ${MONITOR}.cpp
	${CC} -c ${CPPFLAGS} ${MONITOR}.cpp -o ${MONITOR}.o

# Linkado
${MONITOR}: ${TUPLE}.o ${MONITOR}.o  
	${CC} ${TUPLE}.o ${MONITOR}.o -o ${MONITOR} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# DRIVER
# Compilacion
${DRIVER}.o: ${DRIVER}.hpp ${DRIVER}.cpp
	${CC} -c ${CPPFLAGS} ${DRIVER}.cpp -o ${DRIVER}.o

# Linkado
${DRIVER}: ${TUPLE}.o ${SOCKET}.o ${DRIVER}.o  
	${CC} ${LDFLAGS} ${TUPLE}.o ${SOCKET}.o ${DRIVER}.o -o ${DRIVER} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# CLIENTE
# Compilacion
${BASIC_EXAMPLE}.o: ${BASIC_EXAMPLE}.cpp
	${CC} -c ${CPPFLAGS} ${BASIC_EXAMPLE}.cpp -o ${BASIC_EXAMPLE}.o

# Linkado
${BASIC_EXAMPLE}: ${DRIVER}.o ${TUPLE}.o ${SOCKET}.o ${BASIC_EXAMPLE}.o  
	${CC} ${LDFLAGS} ${DRIVER}.o ${TUPLE}.o ${SOCKET}.o ${BASIC_EXAMPLE}.o -o ${BASIC_EXAMPLE} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# OTROCLIENTE
# Compilacion
${INTERACTIVE_EXAMPLE}.o: ${INTERACTIVE_EXAMPLE}.cpp 
	${CC} -c ${CPPFLAGS} ${INTERACTIVE_EXAMPLE}.cpp -o ${INTERACTIVE_EXAMPLE}.o

# Linkado
${INTERACTIVE_EXAMPLE}: ${DRIVER}.o ${TUPLE}.o ${SOCKET}.o ${INTERACTIVE_EXAMPLE}.o  
	${CC} ${LDFLAGS} ${DRIVER}.o ${TUPLE}.o ${SOCKET}.o ${INTERACTIVE_EXAMPLE}.o -o ${INTERACTIVE_EXAMPLE} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# SERVIDOR
# Compilacion
${SERVER}.o: ${SERVER}.cpp 
	${CC} -c ${CPPFLAGS} ${SERVER}.cpp -o ${SERVER}.o

# Linkado
${SERVER}: ${TUPLE}.o ${MONITOR}.o ${SOCKET}.o ${SERVER}.o
	${CC} ${LDFLAGS} ${TUPLE}.o ${MONITOR}.o ${SOCKET}.o ${SERVER}.o -o ${SERVER} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# SOCKETS
# Compilacion libreria de Sockets
${SOCKET}.o: ${SOCKET}.hpp ${SOCKET}.cpp
	${CC} -c ${CPPFLAGS} ${SOCKET}.cpp -o ${SOCKET}.o
#-----------------------------------------------------------
# TUPLE
# Compilacion
${TUPLE}.o: ${TUPLE}.hpp  ${TUPLE}.cpp 
	${CC} -c ${CPPFLAGS} ${TUPLE}.cpp -o ${TUPLE}.o
#-----------------------------------------------------------		
# LIMPIEZA
clean:
	$(RM) ${SOCKET}.o
	$(RM) ${TUPLE}.o
	$(RM) ${BASIC_EXAMPLE} ${BASIC_EXAMPLE}.o
	$(RM) ${INTERACTIVE_EXAMPLE} ${INTERACTIVE_EXAMPLE}.o
	$(RM) ${SERVER} ${SERVER}.o
	$(RM) ${DRIVER} ${DRIVER}.o
	$(RM) ${MONITOR} ${MONITOR}.o
