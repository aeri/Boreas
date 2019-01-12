#include <sstream>
#include "tuplas.hpp"

using namespace std;

/*
 * Pre: <<p1>> es una cadena de caracteres que representa la componente a 
 *       almacenar en la tupla
 * Post: Crea una tupla de dimensión 1 cuyo única componente es <<p1>>
 */
Tupla::Tupla(string p1)
{
    dimension = 1;
    vector[0] = p1;
};


/*
 * Pre: <<p1>> y <<p2>> son dos cadenas de caracteres que representan las componentes a 
 *       almacenar en la tupla
 * Post: Crea una tupla de dimensión 2 cuyas componentes son <<p1>> y <<p2>>
 */
Tupla::Tupla(string p1, string p2)
{
    dimension = 2;
    vector[0] = p1;
    vector[1] = p2;
};

/*
 * Pre: <<p1>> , <<p2>> y <<p3>> son tres cadenas de caracteres que representan las componentes a 
 *       almacenar en la tupla
 * Post: Crea una tupla de dimensión 3 cuyas componentes son <<p1>>, <<p2>> y <<p3>>
 */
Tupla::Tupla(string p1, string p2, string p3)
{
    dimension = 3;
    vector[0] = p1;
    vector[1] = p2;
    vector[2] = p3;
};

/*
 * Pre: <<p1>> , <<p2>>, <<p3>> y <<p4>> son cuatro cadenas de caracteres que representan las componentes a 
 *       almacenar en la tupla
 * Post: Crea una tupla de dimensión 4 cuyas componentes son <<p1>>, <<p2>>, <<p3>> y <<p4>>
 */
Tupla::Tupla(string p1, string p2, string p3, string p4)
{
    dimension = 4;
    vector[0] = p1;
    vector[1] = p2;
    vector[2] = p3;
    vector[3] = p4;
};

/*
 * Pre: <<p1>> , <<p2>>, <<p3>>, <<p4>> y <<p5>> son cinco cadenas de caracteres que representan las componentes a 
 *       almacenar en la tupla
 * Post: Crea una tupla de dimensión 5 cuyas componentes son <<p1>>, <<p2>>, <<p3>>, <<p4>> y <<p5>>
 */
Tupla::Tupla(string p1, string p2, string p3, string p4, string p5)
{
    dimension = 5;
    vector[0] = p1;
    vector[1] = p2;
    vector[2] = p3;
    vector[3] = p4;
    vector[4] = p5;
};

/*
 * Pre: <<p1>> , <<p2>>, <<p3>>, <<p4>>, <<p5>> y <<p6>> son seis cadenas de caracteres que representan las 
 *      componentes a almacenar en la tupla
 * Post: Crea una tupla de dimensión 6 cuyas componentes son <<p1>>, <<p2>>, <<p3>>, <<p4>>, <<p5>> y <<p6>>
 */
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

/*
 * Pre: 1 <= d <= 6 son 
 * Post: Devuelve una tupla cuya dimensión es igual al valor <<d>>
 */
Tupla::Tupla(int d)
{
    dimension = d;
};

/*
 * Pre: <<t>> es una cadena de caracteres que representa una tupla con el formato [x1], [x1,x2], [x1,x2,x3]
        [x1,x2,x3,x4], [x1,x2,x3,x4,x5] o [x1,x2,x3,x4,x5,x6]
 * Post: Devuelve una tupla en la que se han insertado en sus compoenetr
 */
void Tupla::from_string(string t)
{
    // Creación de flujo a partir del string <<t>>
    stringstream ss(t);

    
    bool incorrecto = false;
    int i = dimension;
    
    // Extracción del primer caracter asociado al flujo de la cadena
    if(ss.get() == '[')
    {
        // La cadena tiene formato correcto
        string token;
        while(i > 1)
        // Se han insertado i-1 componentes 
        {
            // Se extraen los caracteres hasta la coma
            getline(ss, token, ',');
            // Se inserta la subcadena como componente i-ésima de la tupla
            vector[dimension - i] = token;
            i--;
            // se han insertado i componentes
        }
        // Terminación de creación de la tupla
        getline(ss, token, ']');
        vector[dimension-1] = token;
        
    }
    else
    {
        // Formato de tupla incorrecto
        incorrecto = true;
    }

    if(incorrecto)
    {
        // Aviso de error 
        cerr << "El formato de entrada es incorrecto" << endl;
    }
};

/*
 * Pre: ---
 * Post: Convierte una tupla en formato de cadena de caracteres
 */
string Tupla::to_string()
{
    string cadena = "[";
    // Se han insertado en la cadena i-1 componentes de la tupla
    for(int i = 0; i < dimension - 1; i++)
    {
        // Añade a la cadena la nueva componente de la cadena
        cadena = cadena + vector[i] + ",";
        // Se han insertado las i componentes de la tupla
    }
    cadena = cadena + vector[dimension - 1] + "]";
    return cadena;
};

/*
 * Pre: 1<= indice <= 6
 * Post: Devuelve la componente guardada en el ´indice <<índice>>
 */
string Tupla::get(int indice)
{
    return vector[indice-1];
};

/*
 * Pre: 1 <= indice <= 6 y <<campo>> es una cadena de caracteres
 * Post: Ha guardado en la componente de la tupla cuya posición es <<indice>>
 *       el valor <<campo>>
 */
void Tupla::set(int indice, string campo)
{
    vector[indice-1] = campo;
};

/*
 * Pre: ---
 * Post: Devuelve la dimensión de la tupla
 */
int Tupla::size()
{
    return dimension;
};
