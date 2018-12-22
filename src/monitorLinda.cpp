#include <queue>
#include <thread>
#include "monitorLinda.hpp"

using namespace std;

const int MAX_ATTEMPS = 10;

//-----------------------------------------------------
MonitorLinda::MonitorLinda()
{
   
};

MonitorLinda::~MonitorLinda(){

};

//----------------- Destructor
/*
void MonitorLinda::RemoveNote(Tupla t)
{
    unique_lock<mutex> lck(mtxMonitor);

    

};
*/



/*
void MonitorLinda::ReadNote(Tupla t, Tupla& r, Socket& soc, int& descriptor){
    unique_lock<mutex> lck(mtxMonitor);
    int dimension = t.size();
    string buffer;
    string message;

    message = "ReadNote:" + t.to_string();
        int send_bytes = soc.Send(descriptor, message);

        if(send_bytes == -1){
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            soc.Close(socket_s1);
        }

        // Recibimos la respuesta del servidor  
        int read_bytes = soc.Recv(descriptor, buffer, MESSAGE_SIZE);

       
        while(buffer == "false"){

        if (tamanyo < 4){
            haytupla1.wait(lck);
        }
        else if (tupla >= 4 && tupla < 6){
            haytupla2.wait(lck);
        }
        else{
            haytupla3.wait(lck);
        }

            message = "ReadNote:" + t.to_string();

            send_bytes = soc.Send(descriptor, message);

            if(send_bytes == -1){
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                // Cerramos el socket
                soc.Close(descriptor);
            }
            else{
            // Recibimos la respuesta del servidor  
            read_bytes = soc.Recv(descriptor, buffer, MESSAGE_SIZE);
            }

            

        }

    read_bytes = soc.Recv(descriptor, buffer, MESSAGE_SIZE);
    r.from_string(buffer);
};


*/