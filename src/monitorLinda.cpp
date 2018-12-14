#include <queue>
#include <thread>
#include "Socket.hpp"
#include "monitorLinda.hpp"

using namespace std;

const int MAX_ATTEMPS = 10;

//-----------------------------------------------------
MonitorLinda::MonitorLinda(const string ip1,
                           const int p1,
                           const string ip2,
                           const int p2,
                           const string ip3,
                           const int p3)
{
    int count = 0;
    int socket_s1;
    int socket_s2;
    int socket_s3;

    Socket server1(ip1, p1);
    Socket server2(ip2, p2);
    Socket server3(ip3, p3);

    do
	{
	    // Conexión con el servidor
	    socket_s1 = server1.Connect();
	    socket_s2 = server2.Connect();
	    socket_s3 = server3.Connect();
	    count++;

	    // Si error --> esperamos 1 segundo para reconectar
	    if(socket_s1 == -1 || socket_s2 == -1 || socket_s3 == -1)
		{
		    this_thread::sleep_for(chrono::seconds(1));
		}
	}
    while((socket_s1 == -1 || socket_s2 == -1 || socket_s3 == -1) && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_s1 == -1 || socket_s2 == -1 || socket_s3 == -1)
	{
        cerr << "Error al conectar" << endl;
	}
    else{
        cout << "Servidores conectados" << endl;    
    }
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
void MonitorLinda::PostNote(Tupla t)
{
    unique_lock<mutex> lck(mtxMonitor);
    int dimension = t.size();
    switch(dimension)
	{
	case 1:
    case 2:
    case 3:    
        cout << "<=3";
    case 4:
    case 5:
        cout << "4||5";
	    // coladim1.push(t);
	default:
        cout << "6";
	}
};
