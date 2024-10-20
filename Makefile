CC=g++

# Root folders
LIBS_DIR = lib
BIN_DIR = bin
SRC_DIR = src
EXAMPLES_DIR   = $(SRC_DIR)/examples
SERVER_DIR = $(SRC_DIR)/server

# Linda Server
SERVER = ${SERVER_DIR}/lindaServer
SERVER_BIN= ${BIN_DIR}/lindaServer

# Libs
DRIVER_DIR	= $(LIBS_DIR)/LindaDriver
MONITOR_DIR	= $(LIBS_DIR)/MonitorLinda
SOCKET_DIR	= $(LIBS_DIR)/Socket
TUPLE_DIR	= $(LIBS_DIR)/Tuple

DRIVER  = ${DRIVER_DIR}/LindaDriver
MONITOR	= ${MONITOR_DIR}/monitorLinda
SOCKET	= ${SOCKET_DIR}/Socket
TUPLE	= ${TUPLE_DIR}/Tuple

# Examples
BASIC_EXAMPLE   = ${EXAMPLES_DIR}/example
BASIC_EXAMPLE_BIN= ${BIN_DIR}/example
INTERACTIVE_EXAMPLE = ${EXAMPLES_DIR}/interactiveExample
INTERACTIVE_EXAMPLE_BIN= ${BIN_DIR}/interactiveExample


# Build Flags
CPPFLAGS=-I. -I${SOCKET_DIR} -O2 -std=c++11 -lsockets -Wall
# Linking threads Flags
LDFLAGS=-pthread

all: ${SERVER_BIN} ${BASIC_EXAMPLE_BIN} ${INTERACTIVE_EXAMPLE_BIN}
#----------------------------------------------------------------------------
#SunOS Binary Compatibility
#SOCKETSFLAGS=-lsocket -lnsl
#-----------------------------------------------------------
# MONITOR
# Build
${MONITOR}.o: ${MONITOR}.hpp ${MONITOR}.cpp
	${CC} -c ${CPPFLAGS} ${MONITOR}.cpp -o ${MONITOR}.o

# Linking
${MONITOR}: ${TUPLE}.o ${MONITOR}.o  
	${CC} ${TUPLE}.o ${MONITOR}.o -o ${MONITOR} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# DRIVER
# Build
${DRIVER}.o: ${DRIVER}.hpp ${DRIVER}.cpp
	${CC} -c ${CPPFLAGS} ${DRIVER}.cpp -o ${DRIVER}.o

# Linking
${DRIVER}: ${TUPLE}.o ${SOCKET}.o ${DRIVER}.o  
	${CC} ${LDFLAGS} ${TUPLE}.o ${SOCKET}.o ${DRIVER}.o -o ${DRIVER} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# BASIC_EXAMPLE
# Build
${BASIC_EXAMPLE}.o: ${BASIC_EXAMPLE}.cpp
	${CC} -c ${CPPFLAGS} ${BASIC_EXAMPLE}.cpp -o ${BASIC_EXAMPLE}.o

# Linking
${BASIC_EXAMPLE_BIN}: ${DRIVER}.o ${TUPLE}.o ${SOCKET}.o ${BASIC_EXAMPLE}.o  | bin
	${CC} ${LDFLAGS} ${DRIVER}.o ${TUPLE}.o ${SOCKET}.o ${BASIC_EXAMPLE}.o -o ${BASIC_EXAMPLE_BIN} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# INTERACTIVE_EXAMPLE
# Build
${INTERACTIVE_EXAMPLE}.o: ${INTERACTIVE_EXAMPLE}.cpp 
	${CC} -c ${CPPFLAGS} ${INTERACTIVE_EXAMPLE}.cpp -o ${INTERACTIVE_EXAMPLE}.o

# Linking
${INTERACTIVE_EXAMPLE_BIN}: ${DRIVER}.o ${TUPLE}.o ${SOCKET}.o ${INTERACTIVE_EXAMPLE}.o | bin 
	${CC} ${LDFLAGS} ${DRIVER}.o ${TUPLE}.o ${SOCKET}.o ${INTERACTIVE_EXAMPLE}.o -o ${INTERACTIVE_EXAMPLE_BIN} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# SERVER
# Build
${SERVER}.o: ${SERVER}.cpp 
	${CC} -c ${CPPFLAGS} ${SERVER}.cpp -o ${SERVER}.o

# Linking
${SERVER_BIN}: ${TUPLE}.o ${MONITOR}.o ${SOCKET}.o ${SERVER}.o | bin
	${CC} ${LDFLAGS} ${TUPLE}.o ${MONITOR}.o ${SOCKET}.o ${SERVER}.o -o ${SERVER_BIN} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# SOCKETS
# Build Sockets library
${SOCKET}.o: ${SOCKET}.hpp ${SOCKET}.cpp
	${CC} -c ${CPPFLAGS} ${SOCKET}.cpp -o ${SOCKET}.o
#-----------------------------------------------------------
# TUPLE
# Build
${TUPLE}.o: ${TUPLE}.hpp  ${TUPLE}.cpp 
	${CC} -c ${CPPFLAGS} ${TUPLE}.cpp -o ${TUPLE}.o
#-----------------------------------------------------------		
# CLEAN BUILD FILES
clean:
	$(RM) ${SOCKET}.o
	$(RM) ${TUPLE}.o
	$(RM) ${BASIC_EXAMPLE_BIN} ${BASIC_EXAMPLE}.o
	$(RM) ${INTERACTIVE_EXAMPLE_BIN} ${INTERACTIVE_EXAMPLE}.o
	$(RM) ${SERVER_BIN} ${SERVER}.o
	$(RM) ${DRIVER} ${DRIVER}.o
	$(RM) ${MONITOR} ${MONITOR}.o

bin:
	mkdir -p ${BIN_DIR}