#include "tuplas.hpp"

using namespace std;


Tupla::Tupla(string p1){
	dimension = 1;
	vector [0] = p1;
}

Tupla::Tupla(string p1, string p2){
	dimension = 2;
	vector [0] = p1;
	vector [1] = p2;
}

Tupla::Tupla(string p1, string p2, string p3){
	dimension = 3;
	vector [0] = p1;
	vector [1] = p2;
	vector [3] = p3;
}

Tupla::Tupla(string p1, string p2, string p3, string p4){
	dimension = 4;
	vector [0] = p1;
	vector [1] = p2;
	vector [3] = p3;
	vector [4] = p4;
}

Tupla::Tupla(string p1, string p2, string p3, string p4, string p5){
	dimension = 5;
	vector [0] = p1;
	vector [1] = p2;
	vector [3] = p3;
	vector [4] = p4;
	vector [5] = p5;
}

Tupla::Tupla(string p1, string p2, string p3, string p4, string p5, string p6){
	dimension = 6;
	vector [0] = p1;
	vector [1] = p2;
	vector [3] = p3;
	vector [4] = p4;
	vector [5] = p5;
	vector [6] = p6;
}

Tupla::Tupla(int d){
	dimension = d;
}
/*
void from_string (string t){
	for (int i = 0; i < t.size(); i++){
		if (t.f)
	}
}
*/

string Tupla::to_string (){
	string cadena = "[";
	for (int i = 0; i < dimension; i++){
			cadena = cadena + vector[i];
	}
	cadena = cadena + "]";
	return cadena;
}

string Tupla::get (int indice){
	return vector[indice];
}

void Tupla::set (int indice, string campo){
	vector[indice] = campo;
}

int Tupla::size(){
	return dimension;
}