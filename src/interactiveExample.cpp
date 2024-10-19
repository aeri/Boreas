#include "LindaDriver.hpp"
#include "Tuple.hpp"

#include <chrono>
#include <cstdlib>
#include <signal.h>

const int MAX_LONG_LINEA = 128;

using namespace std;
bool stop = false;


/*
 * Pre: ---
 * Post: Muestra por pantalla un menu con las posibles opciones
 *       que puede realizar el usuario
 */
void presentarMenu() {
  cout << "Interactive example" << endl;
  cout << "1. OUT" << endl;
  cout << "2. IN" << endl;
  cout << "3. RD" << endl;
  cout << "4. RDP" << endl;
  cout << "0. STOP" << endl;
}

void handleSIGINT(int s) {
  cout << endl << "info: signal " << s << " handled." << endl;
  stop = true;
  exit(0);
}

/*
 * Programa que refleja el comportamiento de un cliente interactivo.
 * Comienza solicitando el cṕdigo de operación por teclado al usuario,
 * puede introducir 1 para PostNote, 2 para RemoveNote, 3 para ReadNote
 * y 0 para terminar ejecución. Si el usuario introduce un código distinto
 * del valor 0 pide la dimensión de la tupla y las componentes para formarla
 * al usario, y en caso contario, el cliente cierra conexión con el servidor
 * Linda
 */
int main(int argc, char *argv[]) {

  struct sigaction sigIntHandler;

  // faltan argumentos en la invocación?
  if (argc < 2) {
    cerr << "Start as:" << endl;
    cerr << "   interactiveExample <IP_LS> <Port_LS>" << endl;
    cerr << "      <IP_LS>: Linda server IP address" << endl;
    cerr << "      <Port_LS>: Linda server listen port" << endl;
    return 1;
  }

  sigIntHandler.sa_handler = handleSIGINT;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  // un driver con los datos pasados en la invocación
  LD LindaDriver(argv[1], argv[2]);
  // La conexión con el servidor Linda ya está establecida
  cout << "Connected to server " << argv[1] << " successfully." << endl;

  int operation;
  int dimension;

  // Se presenta por pantalla el menú de opciones
  presentarMenu();

  // mientras no ha finalizado
  while (!stop) {
    // Petición al usuario de tarea a pedir al cliente
    cout << endl << "Select an operation: " << flush;
    cin >> operation;
    string component;

    // si no desea parar la ejecución del cliente
    if (operation != 0) {
      // Petición de dimensión
      cout << "Select dimension: " << flush;
      cin >> dimension;

      // Validación de la dimensión de la tupla
      while (cin.fail() || dimension < 1 || dimension > 6) {
        cin.clear();
        cin.ignore(MAX_LONG_LINEA, '\n');
        cerr << "Invalid dimension" << endl;
        cout << "Select dimension: " << flush;
        cin >> dimension;
      }

      // Constructor de la tupla
      Tuple tta(dimension);

      for (int i = 1; i <= dimension; i++) {

        // Petición de component de tupla al usuario
        cout << "Input component " << i << ": " << flush;
        cin >> component;

        // insertar component en la tupla
        tta.set(i, component);
      }

      if (operation == 1) {
        // el usuario pide hacer PostNote
        cout << "OUT of " << tta.to_string() << endl;
        auto init = chrono::system_clock::now();
        LindaDriver.OUT(tta);
        auto end = chrono::system_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - init);

        cout << "Operation performed in: " << elapsed.count()
             << " milliseconds." << endl;
      } else if (operation == 2) {
        // el usuario pide hacer RemoveNote
        cout << "IN of " << tta.to_string() << endl;
        auto init = chrono::system_clock::now();
        cout << "Returned: " << LindaDriver.IN(tta).to_string() << endl;
        auto end = chrono::system_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - init);

        cout << "Operation performed in: " << elapsed.count()
             << " milliseconds." << endl;
      } else if (operation == 3) {
        // el usuario pide hacer ReadNote
        cout << "RD of " << tta.to_string() << endl;
        auto init = chrono::system_clock::now();
        cout << "Returned: " << LindaDriver.RD(tta).to_string() << endl;
        auto end = chrono::system_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - init);

        cout << "Operation performed in: " << elapsed.count()
             << " milliseconds." << endl;
      } else if (operation == 4) {
        bool found;
        // el usuario pide hacer ReadNote
        cout << "RD NOT blocking of " << tta.to_string() << endl;
        auto init = chrono::system_clock::now();

        string tupla = LindaDriver.RDP(tta, found).to_string();

        if (found) {
          cout << "Returned: " << tupla << endl;
        } else {
          cout << "Tuple not found." << endl;
        }

        auto end = chrono::system_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - init);

        cout << "Operation performed in: " << elapsed.count()
             << " milliseconds." << endl;
      }
    } else {
      // Se cierra conexión con el servidor Linda
      cout << "Send bye to the server." << endl;
      LindaDriver.STOP();
      stop = true;
    }
  }

  return 0;
}
