#include <sstream>
#include "tuplas.hpp"

using namespace std;

Tupla::Tupla(string p1)
{
    dimension = 1;
    vector[0] = p1;
};

Tupla::Tupla(string p1, string p2)
{
    dimension = 2;
    vector[0] = p1;
    vector[1] = p2;
};

Tupla::Tupla(string p1, string p2, string p3)
{
    dimension = 3;
    vector[0] = p1;
    vector[1] = p2;
    vector[2] = p3;
};

Tupla::Tupla(string p1, string p2, string p3, string p4)
{
    dimension = 4;
    vector[0] = p1;
    vector[1] = p2;
    vector[2] = p3;
    vector[3] = p4;
};

Tupla::Tupla(string p1, string p2, string p3, string p4, string p5)
{
    dimension = 5;
    vector[0] = p1;
    vector[1] = p2;
    vector[2] = p3;
    vector[3] = p4;
    vector[4] = p5;
};

Tupla::Tupla(string p1, string p2, string p3, string p4, string p5, string p6)
{
    dimension = 6;
    vector[0] = p1;
    vector[1] = p2;
    vector[2] = p3;
    vector[3] = p4;
    vector[4] = p5;
    vector[5] = p6;
};

Tupla::Tupla(int d)
{
    dimension = d;
};

void Tupla::from_string(string t)
{
    stringstream ss(t);

    bool incorrecto = false;
    int i = dimension;
    if(ss.get() == '[')
	{
	    string token;
	    while(i > 1)
		{
		    getline(ss, token, ',');
		    //cout << token << endl;
		    vector[i] = token;
		    i--;
		}
	    getline(ss, token, ']');
	    vector[i - 1] = token;
	    //cout << token << endl;
	}
    else
	{
	    incorrecto = true;
	}

    if(incorrecto)
	{
	    cerr << "El formato de entrada es incorrecto" << endl;
	}
};

string Tupla::to_string()
{
    string cadena = "[";
    for(int i = 0; i < dimension - 1; i++)
	{
	    cadena = cadena + vector[i] + ",";
	}
    cadena = cadena + vector[dimension - 1] + "]";
    return cadena;
};

string Tupla::get(int indice)
{
    return vector[indice];
};

void Tupla::set(int indice, string campo)
{
    vector[indice] = campo;
};

int Tupla::size()
{
    return dimension;
};