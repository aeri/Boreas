#include "LindaDriver.hpp"
#include "Tuple.hpp"
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

// Definición de constantes
const int END_OF_SERVICE = 0;
const int COTA_SUP = 3;
const int MAX_DIMENSION = 6;
const int BASE_DECIMAL = 10;
const int TIPO_CAMPO = 5;
const int MAX_ESPACIO_ALFABETO = 26;
const string comodin = "?";


/*
 * Pre: ---
 * Post: Presenta por pantalla un listado con
 *       las posibles ordenes que puede ejecutar el usuario
 */
void presentarMenu(){
  cout << endl;
  cout << "  Menu de opciones " << endl;
  cout << " ================== " << endl;
  cout << " 1 --> ejecutar operacion " << endl;
  cout << " 0 --> finalizar ejecución de cliente " << endl << endl;
}

/*
 * Pre: <<o>> es un valor entero que indica una orden
        introducida por el usuario
 * Post: Devuelve <<true>> si y solo si la orden es valida, es decir,
 *       si esa orden es 0 o 1. Devuelve <<false>> en caso contrario
 */
bool esOrdenValida(int& o){
  return (o == END_OF_SERVICE || o == END_OF_SERVICE + 1);
}

/*
 * Fichero cliente Mixto que pide solicitar al usuario por teclado una orden.
 * La orden es un entero que puede ser 0 o 1. Si la orden es 1 el cliente de
 * manera aleatoria selecciona una operacion PN, ReadNote o RN y genera una
 * tupla con la que trabajar. Tras efectuar la tarea vuelve a pedir orden al
 * usuario. Si la orden es 0 detiene la ejecución, y si la orden es distita de
 * de 0 y 1 informa de operacion desconocida y la solicita de nuevo
 *
 */
int main(int argc, char* argv[]){

  if(argc < 2){
    cerr << "Invocar como:" << endl;
    cerr << "      Cliente Mixto <IP_LS> <Port_LS>" << endl;
    cerr << "      <IP_LS>: IP del servidor Linda" << endl;
    cerr << "      <Port_LS>: puerto del servidor Linda" << endl;
    return 1;
  }

  // un driver con los datos pasados en la invocación
  LD LindaDriver(argv[1], argv[2]);
  // La conexión con el servidor Linda ya está establecida
  cout << "Conexión al servidor " << argv[1] << " establecida..." << endl;

  // Orden a introducir los usuarios por teclado
  int orden;

  // Presentación del menú de opciones del Cliente
  presentarMenu();

  cout << " Introduzca una orden " << flush;
  cin >> orden;

  // Orden distinta de 0
  while (orden != END_OF_SERVICE ){

    // Bucle de validación de la orden introducida por el usuario
    while (!esOrdenValida(orden)){
        // La orden el usuario no es valida
        cout << " La orden " << orden << " no esta contemplada " << endl;
        cout << " Introduzca una orden " << flush;
        cin >> orden;
    }

    cout << "Orden correcta" << endl;
    // Generador de numeros aleatorios
    srand(time(NULL));

    // codigo_OP es un numero aleatorio entre 0 y 3
    int codigo_OP = rand() % COTA_SUP;

    // Seleccion de la dimension de la tupla aleatoria entre 0 y 6
    int dimension = (rand() % MAX_DIMENSION) + 1 ;

    // Cadena de caracteres que guarda la componente de la tupla a insertar
    string componente;

    // Cadena de caracteres que guarda el valor aleatorio generado para
    // insertar en la tupla
    string valor;

    // Constructor de la tupla
    Tuple t(dimension);

    cout << "dimension: " << dimension << endl;
    cout << "codigo op: " << codigo_OP << endl;
  
  // Tipo de componente a introducir
  int codigo_COMP;
  
    for (int i = 1; i <= dimension; i++){

            cout << "iteracion numero: " << i << endl;

            if (codigo_OP == 0){
              // es PostNote
              // Calculo del tipo de componente a meter 0-3
              codigo_COMP = rand() % (TIPO_CAMPO - 2);
            }
            else {
              // no es PostNote
              // no se pueden meter componentes con ?(A-Z)
              // Calculo del tipo de componente a meter 0-4
              codigo_COMP = rand() % (TIPO_CAMPO -1 );
            }

            cout << "codigo_COMP: " << codigo_COMP << endl;

            // Numero de caracteres de la cadena de 1 a 10
            int dim_campo = rand() % BASE_DECIMAL + 1;

            cout << "dim_campo: " << dim_campo << endl;

            // Vaciado de la componente para limipar los datos antiguos
            componente.clear();

            // Borrado del dato generado de forma automatica
            valor.clear();

            cout << "Clears completados satisfactoriamente" << endl;
            switch (codigo_COMP) {
              case 0:
                    // Se rellena la componente con dim_campo numeros
                    for (int j =1; j <= dim_campo; j++){
                        valor = rand() % BASE_DECIMAL + '0';
                        componente = componente + valor;
                        cout << "componente: " << componente << endl;
                    }
                    break;
              case 1:
                    // Se rellena la componente con dim_campo mayusculas
                    for (int j = 1; j <= dim_campo; j++){
                        valor = rand() % MAX_ESPACIO_ALFABETO + 'A';
                        componente = componente + valor;
                        cout << "componente: " << componente << endl;
                    }
                    break;
              case 2:
                    // Se rellena la componente con dim_campo minusculas
                    for (int j = 1; j <= dim_campo; j++){
                        valor = rand() % MAX_ESPACIO_ALFABETO + 'a';
                        componente = componente + valor;
                        cout << "componente: " << componente << endl;
                    }
                    break;
              case 3:
                    // Se rellena con una variable comodín
          
                    valor = (rand() % MAX_ESPACIO_ALFABETO + 'A');
          valor = comodin + valor;
                    componente = componente + valor;
                    cout << "componente: " << componente << endl;
                    break;
              default:
                    // Error en el formato de la componente
                    cerr << " Error en el formato de componente " << endl;
            }
            // La componente a insertar en la tupla ya esta obtenida
            cout << "componente final:" << componente << endl;
            t.set(i, componente);
      }
      // La componente se ha insertado
      cout<< "####INSTERTADA####" << endl;

      // Tratamiento de la operacion a enviar por parte del cliente
      switch (codigo_OP) {
        case 0:
              cout << "Cliente Mixto realiza PostNote de " << t.to_string() << endl;
              LindaDriver.PN(t);
              break;
        case 1:
              cout << "Cliente Mixto realiza RemoveNote de " << t.to_string() << endl;
              cout << "Se devuelve: " << LindaDriver.RN(t).to_string() << endl;
              break;
        case 2:
              cout << "Cliente Mixto realiza ReadNote de " << t.to_string() << endl;
              cout << "Se devuelve: " << LindaDriver.RD(t).to_string() << endl;
              break;
        default:
              cerr << " Error en el codigo de operacion" << endl;
      }

      // Transaccion finalizada y vuelve a solicitar orden
      presentarMenu();

      cout << " Introduzca una orden " << flush;
      cin >> orden;

      // Orden introducida por el usuario
  }

  // El usuario termina la ejecución del Cliente porque orden = 0
  // Cierra conexión con Linda
  cout << " Ejecución finalizada" << endl;
  LindaDriver.STOP();

  return 0;
}
