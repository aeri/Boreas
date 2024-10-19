#include <cstring>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "Tuple.hpp"
#include "monitorLinda.hpp"

bool STOP = false;
string NOT_FOUND_ERROR("NOT_FOUND");

const char FIN_MSG[] = "END OF SERVICE";
regex e("(?:OUT|IN|RD|RDP):\\[[^\\],\n]+?(?:,[^\\],\n]+?)*\\]");

int getTupleDimension(string s) {
  stringstream ss(s);
  int dimension = 1;
  for (unsigned int i = 0; i < s.size(); i++) {
    if (ss.get() == ',') {
      ++dimension;
    }
  }
  return dimension;
}

void handleSIGINT(int s) {
  cout << endl << "info: signal " << s << " handled." << endl;
  STOP = true;
}

void handleClient(int clientSocket, MonitorLinda &ML) {

  int length = 1024;
  char buffer[1024] = {0};

  bool out = false;
  while (!out) {
    bzero(buffer, length);
    int bytesRead = read(clientSocket, buffer, length);
    if (bytesRead > 0) {

      string request(buffer);
      string response;

      if (regex_match(request, e)) {

        cout << "info: string message matched: " << request << endl;
        stringstream ss(request);
        char del = ':';

        string operation;
        getline(ss, operation, del);
        string tuple;
        getline(ss, tuple, del);

        int ssize = getTupleDimension(tuple);
        Tuple t(ssize);
        t.from_string(tuple);

        // Operation processing

        if (operation == "OUT") {
          ML.PostNote(t);
          response = "OK";
        }

        else if (operation == "RD") {
          Tuple r(ssize);
          ML.ReadNote(t, r, true);
          response = r.to_string();
          cout << "info: RD resolves with: " << operation << endl;
        }

        else if (operation == "RDP") {
          Tuple r(ssize);
          ML.ReadNote(t, r, false);
          response = r.to_string();

          if (r.get(1).compare(NOT_FOUND_ERROR) == 0) {
            response = NOT_FOUND_ERROR;
          } else {
            response = r.to_string();
          }

          cout << "info: RDP resolves with: " << response << endl;
        } else if (operation == "IN") {
          Tuple r(ssize);
          ML.RemoveNote(t, r);
          response = r.to_string();
          cout << "info: IN resolves with: " << response << endl;
        }
        // client response stored in <<response>>
        // IF OUT sends OK
        // IF RD or IN sends requested tuple
        int send_bytes =
            send(clientSocket, response.c_str(), response.length(), 0);

        if (send_bytes == -1) {
          string errorMsg(strerror(errno));
          cerr << "error: failure to send data to Linda server: " + errorMsg
               << endl;
          close(clientSocket);
          exit(1);
        }

        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(clientSocket, SOL_SOCKET, SO_ERROR, &error, &len) == -1) {
            std::cerr << "error: retrieving socket: " << strerror(errno) << std::endl;
        }

        if (error != 0) {
            std::cerr << "error: after send: " << strerror(error) << std::endl;
        }
      }

      else if (strncmp(buffer, FIN_MSG, length) == 0) {
        cout << "info: end of service requested" << endl;
        out = true;
      } else {
        cerr << "error: operation not recognized " << request << endl;
        out = true;
      }
    }
  }
  cout << "info: client disconnected with out " << out << endl;
  close(clientSocket); // Close connection
}

int main(int argc, char *argv[]) {
  struct sigaction sigIntHandler;

  MonitorLinda ML;

  int serverSocket, clientSocket;
  struct sockaddr_in serverAddress, clientAddress;
  int addrlen = sizeof(clientAddress);

  // Prevent SIGPIPEs caused by sockets
  signal(SIGPIPE, SIG_IGN);

  sigIntHandler.sa_handler = handleSIGINT;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  if (argc < 2) {
    cerr << "Start as:" << endl;
    cerr << "   lindaServer <Port_LS>" << endl;
    cerr << "      <Port_LS>: Linda server listen port" << endl;
    return 1;
  }

  int SERVER_PORT = atoi(argv[1]);

  cout << R"(
	 __  __  __  __     __ 
	|__)/  \|__)|_  /\ (_  
	|__)\__/| \ |__/--\__) 
                       
	github.com/aeri/boreas
	Licensed under the GNU General Public License v3.0 
	)"
       << endl;

  // Create server socket
  if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    cerr << "error: failure to create socket" << endl;
    return -1;
  }

  // Server management (IP address and port)
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(SERVER_PORT);

  // Bind socket with port
  if (bind(serverSocket, (struct sockaddr *)&serverAddress,
           sizeof(serverAddress)) < 0) {
    cerr << "error: failure to bind socket" << endl;
    close(serverSocket);
    return -1;
  }

  // Listen on port
  if (listen(serverSocket, 3) < 0) {
    cerr << "error: failure to listen on port " << SERVER_PORT << endl;
    close(serverSocket);
    return -1;
  }

  cout << "info: starting Linda server on port " << SERVER_PORT << endl;

  // Attend client requests
  while (!STOP) {
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress,
                               (socklen_t *)&addrlen)) < 0) {
      cerr << "error: failure to accept connection" << endl;
      continue;
    } else {

      char *s = inet_ntoa(clientAddress.sin_addr);
      cout << "info: handle new client from " << s << endl;

      // Handle client request in a new thread
      thread(&handleClient, ref(clientSocket), ref(ML)).detach();
    }
  }
  return 0;
}
