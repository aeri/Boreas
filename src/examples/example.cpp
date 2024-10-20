#include "../../lib/LindaDriver/LindaDriver.hpp"
#include <cstring>

using namespace std;

//-----------------------------------------------------
int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Start as:" << endl;
    cerr << "   example <IP_LS> <Port_LS>" << endl;
    cerr << "      <IP_LS>: Linda server IP address" << endl;
    cerr << "      <Port_LS>: Linda server listen port" << endl;
    return 1;
  }

  // invoke LindaDriver and connect to the server
  LD LindaDriver(argv[1], argv[2]);
  // Connected to server

  cout << "Connected successfully to Linda server" << endl;

  Tuple t1("1", "username", "token");
  Tuple t2("7777");
  Tuple t3("entry", "el", "public", "45", "34", "88");
  Tuple t4("entry", "state", "public", "XXI", "34", "public");

  LindaDriver.OUT(t1);
  LindaDriver.OUT(t2);

  Tuple ttp("?X", "username", "token");

  cout << "Received tuple: " << LindaDriver.RD(ttp).to_string() << endl;

  LindaDriver.OUT(t2);
  LindaDriver.OUT(t3);
  LindaDriver.OUT(t3);
  LindaDriver.OUT(t3);
  LindaDriver.OUT(t4);

  // shows "username" via stdout
  cout << t1.get(2) << endl;
  // t3.to_string() shows "[entry,el,public,45,34,88]"
  string serial = t3.to_string();
  cout << serial << endl;
  // t3 components are ("a","b","c","45","34","public")
  t3.from_string("[a,b,c,45,34,public]");
  // shows [a,b,c,45,34,public] via stdout
  cout << t3.to_string() << endl;
  LindaDriver.OUT(t3);

  // Creates 3 empty elements tuple "".
  // It is equivalent to Tuple t5("","","")
  Tuple t5(3);
  t5.set(2, "hola");
  t5.set(3, "Mundo");
  LindaDriver.OUT(t5);
  // shows [,hola,Mundo] via stdout
  cout << t5.to_string() << endl;
  // shows 3 elements
  cout << "t5 has " << t5.size() << " elements" << endl;

  // Patters can be used, it works like a wildcard, using the character "?"
  // in one ore more tuple positions
  // The reserverd character "?" cannot be a tuple component
  // Their representation could be "??"

  Tuple p1("?X");
  Tuple p2("a", "?X", "c", "?Y", "34", "?Z");
  Tuple res1(1), res2(p2.size());

  res1 = LindaDriver.IN(p1);        // res1 takes t2 value
  res2 = LindaDriver.IN(p2);        // res2 takes t3 value
  cout << res1.to_string() << endl; // shows [7777]
  cout << res2.to_string() << endl; // shows [a,b,c,45,34,public]

  Tuple *v[2];
  v[0] = new Tuple("Alpha", "7777");
  v[1] = new Tuple("Beta", "7777", "venus");

  delete v[0];
  delete v[1];

  LindaDriver.STOP();

  return 0;
}
