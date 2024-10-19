/*
 * Tuple storage monitor implementation
 */

#include "monitorLinda.hpp"

using namespace std;

const int NUM_MATRIX = 6;

//-----------------------------------------------------

/*
 * Pre: ---
 * Post:Constructor de la clase monitorLinda
 */
MonitorLinda::MonitorLinda() {
  tupleSpace1.first = nullptr;
  tupleSpace2.first = nullptr;
  tupleSpace3.first = nullptr;
  tupleSpace4.first = nullptr;
  tupleSpace5.first = nullptr;
  tupleSpace6.first = nullptr;
};

/*
 * Pre:
 * Post:Destructor de la clase monitorLinda
 */
MonitorLinda::~MonitorLinda(){};

/*
 * Pre: <<a>> y <<b>> son dos strings que respetan el formato de las tuplas
 * ["",""] Post: Devuelve <<true>> si <<a>> y <<b>> tienen las mismas
 * componentes, teninedo en cuenta que una componente ?x coincide con cualquier
 * valor. Devuelve <<false>> en caso contario
 */
bool areEqual(string a, string b) {
  if (b[0] == '?' && b[1] >= 'A' && b[1] <= 'Z') {
    return true;
  } else {
    return a == b;
  }
}

/*
 * Pre: La dimensión de la tupla t es >=1 && <=6
 * Post: Añade a la matriz correspondiente la tupla t
 */
void MonitorLinda::PostNote(Tuple t) {
  unique_lock<mutex> lck(mtxMonitor);

  bbdd::Node *np = new bbdd::Node;

  // Obtención de la dimensión de la tupla
  int dimension = t.size();

  // Determinación en que matriz se va a insertar la tupla <<t>>
  // mandada por el cliente
  switch (dimension) {
  case 1:
    np->nextTuple = tupleSpace1.first;
    tupleSpace1.first = np;
    break;
  case 2:
    np->nextTuple = tupleSpace2.first;
    tupleSpace2.first = np;
    break;
  case 3:
    np->nextTuple = tupleSpace3.first;
    tupleSpace3.first = np;
    break;
  case 4:
    np->nextTuple = tupleSpace4.first;
    tupleSpace4.first = np;
    break;
  case 5:
    np->nextTuple = tupleSpace5.first;
    tupleSpace5.first = np;
    break;
  case 6:
    np->nextTuple = tupleSpace6.first;
    tupleSpace6.first = np;
    break;
  default:
    cerr << "mon-error: tuple dimension > 6 in PN" << endl;
  }

  // Recorrido de las componentes de la tupla
  // Se asigna a cada row de la matriz la componente i-ésima de la tupla
  for (int i = 1; i <= dimension; ++i) {
    // Asignación de la componente
    np->valor = t.get(i);

    if (i != dimension) {
      // Reserva de memoria para la siguiente componente
      // Desplazamiento al nuevo puntero
      np->nextComp = new bbdd::Node;
      np = np->nextComp;
    } else {
      // Final de la tupla
      np->nextComp = nullptr;
    }
  }

  // Tras añadir la tupla <<t>> se avisa a los clientes bloqueados
  // que están esperando una tupla de dimensión igual a la de la tupla <<t>>
  switch (dimension) {
  case 1:
    new_tuple_1.notify_all();
    break;
  case 2:
    new_tuple_2.notify_all();
    break;
  case 3:
    new_tuple_3.notify_all();
    break;
  case 4:
    new_tuple_4.notify_all();
    break;
  case 5:
    new_tuple_5.notify_all();
    break;
  case 6:
    new_tuple_6.notify_all();
    break;
  default:
    // Error
    cerr << "mon-error: failure at notify: tuple dimension > 6" << endl;
  }
}

/*
 * Pre: La dimensión de la tupla t es >=1 && <=6
 * Post: Busca la tupla t en la matriz correspondiente, la guarda en la tupla
 * <<r>> y la borra de la matriz de tuplas correspondiente
 */
void MonitorLinda::RemoveNote(Tuple t, Tuple &r) {
  unique_lock<mutex> lck(mtxMonitor);

  // Punteros auxiliares
  bbdd::Node *row;
  bbdd::Node *column;
  bbdd::Node *aux;

  bbdd::Node *pr;

  // Obtención de la dimensión
  const int dimension = t.size();

  // A partir de la dimensión de la tupla <<t>> se
  // determina la matriz de la que se va a extrarer
  switch (dimension) {
  case 1:
    row = tupleSpace1.first;
    break;
  case 2:
    row = tupleSpace2.first;
    break;
  case 3:
    row = tupleSpace3.first;
    break;
  case 4:
    row = tupleSpace4.first;
    break;
  case 5:
    row = tupleSpace5.first;
    break;
  case 6:
    row = tupleSpace6.first;
    break;
  default:
    row = nullptr;
    cerr << "mon-error: failure at get matrix (RN): tuple dimension > 6"
         << endl;
  }

  // Asignación de punteros al punto de inicio de búsqueda
  column = row;
  aux = row;
  pr = row;

  bool found = false;            // matriz encontrada
  bool continueSearching = true; // matrices iguales
  int i;

  bool searched = false; // ningún cliente ha solicitado la tupla <<t>>

  // la tupla no se ha found todavía en la matriz
  while (!found) {
    // mientras no quedan tuplas por recorrer en la matriz y
    // no esté en posesión de otro cliente
    while (searched || column == nullptr) {
      // La tupla no está disponible
      cout << "mon-info: tuple not found in tuples space (RN)" << endl;
      // Se duerme el cliente en cola la asociada a la variable condición
      // correcta Se determina en función de la dimensión de la tupla que ha
      // pedido el cliente
      switch (dimension) {
      case 1:
        new_tuple_1.wait(lck);
        row = tupleSpace1.first;
        break;
      case 2:
        new_tuple_2.wait(lck);
        row = tupleSpace2.first;
        break;
      case 3:
        new_tuple_3.wait(lck);
        row = tupleSpace3.first;
        break;
      case 4:
        new_tuple_4.wait(lck);
        row = tupleSpace4.first;
        break;
      case 5:
        new_tuple_5.wait(lck);
        row = tupleSpace5.first;
        break;
      case 6:
        new_tuple_6.wait(lck);
        row = tupleSpace6.first;
        break;
      default:
        cerr << "mon-error: failure at wait (RN): tuple dimension > 6" << endl;
      }
      column = row;
      aux = row;
      pr = row;
    }

    // La tupla está disponible y hay que econtrarla
    while (!found && column != nullptr) {
      continueSearching = true;
      i = 1;

      // Bucle de búsqueda de tupla
      while (continueSearching && row != nullptr) {
        // Evaluamos la componente de la tupla con la actual
        continueSearching = areEqual(row->valor, t.get(i));

        // Si son iguales puede ser la tupla que el cliente ha pedido
        if (continueSearching) {
          // Inserción de la componente en la tupla a enviar al cliente
          // Desplazamiento a la siguiente componente
          r.set(i, row->valor);
          row = row->nextComp;
          ++i;
        }
      }
      if (row == nullptr && continueSearching) {
        found = true;
        aux->nextTuple = column->nextTuple;

        /* Se comienza el borrado de elementos
            - Si el elemento a borrar el first hay que tener en cuenta
                    que el puntero a first tendrá que actualizarse por
                    el valor que tenga el puntero al siguiente que apunta
        */

        // Tratamiento especial si se borra la primera tupla
        if (column == pr) {
          cout << "mon-info: deleting first element of tuples space" << endl;
          // Se debe desplazar el puntero de inicio de la matriz a la
          // tupla siguiente para no perder los restantes
          switch (dimension) {
          case 1:
            tupleSpace1.first = column->nextTuple;
            break;
          case 2:
            tupleSpace2.first = column->nextTuple;
            break;
          case 3:
            tupleSpace3.first = column->nextTuple;
            break;
          case 4:
            tupleSpace4.first = column->nextTuple;
            break;
          case 5:
            tupleSpace5.first = column->nextTuple;
            break;
          case 6:
            tupleSpace6.first = column->nextTuple;
            break;
          default:
            cerr << "mon-error: failure at delete first element: tuple "
                    "dimension > 6"
                 << endl;
          }
        }
        // Borrado de la tupla <<t>> pedia por el cliente en la matriz
        bbdd::Node *saux;
        while (column != nullptr) {
          saux = column;
          column = column->nextComp;
          // cout << "Se borra la componente: " << saux->valor << endl;
          delete (saux);
          saux = nullptr;
        }
      } else {
        // si no coincide la primera componente de la tupla se pasa a la
        // siguiente porque no tiene sentido seguir buscando
        aux = column;
        column = column->nextTuple;
        row = column;
      }
    }
  }
  // Ha habido un cliente que ha buscado esa tupla
  searched = true;
}

/*Pre:la dimensión de la tupla t es >=1 && <=6
 *Post:Busca la tupla t en la matriz correspondiente y actualiza el valor de
 *dicha tupla
 */

void MonitorLinda::ReadNote(Tuple t, Tuple &r, bool locked) {

  unique_lock<mutex> lck(mtxMonitor);

  // Punteros auxiliares
  bbdd::Node *row;
  bbdd::Node *column;

  // Obtención de la dimensión de la tupla
  int dimension = t.size();

  // Determinación de en que matriz se va a trabajar en función de
  // la dimensión de la tupla
  switch (dimension) {
  case 1:
    row = tupleSpace1.first;
    break;
  case 2:
    row = tupleSpace2.first;
    break;
  case 3:
    row = tupleSpace3.first;
    break;
  case 4:
    row = tupleSpace4.first;
    break;
  case 5:
    row = tupleSpace5.first;
    break;
  case 6:
    row = tupleSpace6.first;
    break;
  default:
    row = nullptr;
    cerr << "mon-error: failure at get matrix (ReadN): tuple dimension > 6"
         << endl;
  }

  column = row;

  bool found = false;     // tupla encontrada
  bool continueSearching; // se continua la búsqueda
  int i;
  bool searched = false; // la tupla no esta en posesión de ningún cliente

  // mientras no la haya found
  while (!found) {
    // mientras la tupla <<t>>  esté en posesión de ningún cliente o
    // no halla tuplas en la matriz el cliente se espera
    while (searched || column == nullptr) {
      if (locked) {
        // gestión del bloqueo del cliente en la cola asociada a la
        // variable condición correspondiente

        // La tupla no se ha found
        cout << "mon-info: tuple not found in tuples space (RD/RX)" << endl;

        switch (dimension) {
        case 1:
          new_tuple_1.wait(lck);
          row = tupleSpace1.first;
          break;
        case 2:
          new_tuple_2.wait(lck);
          row = tupleSpace2.first;
          break;
        case 3:
          new_tuple_3.wait(lck);
          row = tupleSpace3.first;
          break;
        case 4:
          new_tuple_4.wait(lck);
          row = tupleSpace4.first;
          break;
        case 5:
          new_tuple_5.wait(lck);
          row = tupleSpace5.first;
          break;
        case 6:
          new_tuple_6.wait(lck);
          row = tupleSpace6.first;
          break;
        default:
          cerr << "mon-error: failure at wait (RD/RX): tuple dimension > 6"
               << endl;
        }
        column = row;
      } else {
        r.set(1, "NOT_FOUND");
        found = true;
        break;
      }
    }

    // la tupla está en la matriz y no está en posesión de ningún otro cliente
    // Se busca la tupla <<t>> en la matriz
    while (!found && column != nullptr) {
      // Quedan tuplas por recorrer
      continueSearching = true;
      i = 1;
      while (continueSearching && row != nullptr) {
        // Comprobación de si coninciden las componentes
        continueSearching = areEqual(row->valor, t.get(i));
        if (continueSearching) {
          // Inserción de la componente
          // Desplazamiento a la componente siguiente
          r.set(i, row->valor);
          row = row->nextComp;
          ++i;
        }
      }
      // Verificación de si la tupla se ha encntrado
      found = (row == nullptr && continueSearching);

      // Se avanza a la siguiente tupla
      column = column->nextTuple;
      row = column;
    }
  }
  searched = true;
}
