
/*
 * Tuple storage monitor interface
 */

#ifndef _MONITORLINDA_HPP_
#define _MONITORLINDA_HPP_

#include "../Tuple/Tuple.hpp"
#include <condition_variable>
#include <mutex>
#include <string>

using namespace std;

class MonitorLinda {
private:
  struct bbdd {
    struct Node {
      string valor;
      // vertical
      Node *nextComp;
      // horizontal
      Node *nextTuple;
    };
    Node *first;
  };

  bbdd tupleSpace1;
  bbdd tupleSpace2;
  bbdd tupleSpace3;
  bbdd tupleSpace4;
  bbdd tupleSpace5;
  bbdd tupleSpace6;

  mutex mtxMonitor;

  condition_variable new_tuple_1;
  condition_variable new_tuple_2;
  condition_variable new_tuple_3;
  condition_variable new_tuple_4;
  condition_variable new_tuple_5;
  condition_variable new_tuple_6;

public:
  MonitorLinda();

  ~MonitorLinda();

  // add tuple
  void PostNote(Tuple t);
  // search, get and remove tuple
  void RemoveNote(Tuple t, Tuple &r);
  // search and get tuple
  void ReadNote(Tuple t, Tuple &r, bool locked);
};
#endif
