// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include "LRUCacheADDR.h"

int main(int argc, char* argv[]) {
  LRUCache<3> c;
  cout << "WRITE: ADDR=1, DATA=1" << endl;
  c.write(1, 1);
  c.print();
  cout << "WRITE: ADDR=1, DATA=10" << endl;
  c.write(1, 10);
  c.print();
  cout << "WRITE: ADDR=2, DATA=20" << endl;
  c.write(2, 20);
  c.print();
  cout << "WRITE: ADDR=3, DATA=30" << endl;
  c.write(3, 30);
  c.print();
  cout << "WRITE: ADDR=4, DATA=40" << endl;
  c.write(4, 40);
  c.print();

  int value;
  c.read(2, &value);
  cout << "READ: ADDR=2, DATA-> " << value << endl;
  c.print();
  c.read(3, &value);
  cout << "READ: ADDR=3, DATA-> " << value << endl;
  c.print();
  c.read(2, &value);
  cout << "READ: ADDR=2, DATA-> " << value << endl;
  c.print();
  return 0;
}
