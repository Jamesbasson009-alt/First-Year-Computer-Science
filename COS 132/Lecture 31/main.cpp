
#include <iostream>

#include <sstream>



int main() {

  int treasure = 42;

  int* treasurePtr = &treasure;



  std::stringstream ss;

  ss << treasurePtr; // Alex tries to stringify the pointer



  int** mapPtr;

  ss >> mapPtr; // Then tries to read it back as int**



  // Pixel tries to follow the map...

  std::cout << "Treasure value via mapPtr: " << **mapPtr << std::endl;



  return 0;

}

