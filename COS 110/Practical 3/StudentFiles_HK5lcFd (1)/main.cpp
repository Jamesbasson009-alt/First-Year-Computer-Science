#include <iostream>

#include "Factory.h"
#include "Worker.h"
int main (){
    Factory* factory = new Factory();
    *factory|"file.txt";
    Worker * worker1 = new Worker("Jane", 540.34);
    Worker * worker2 = new Worker("Tarzan", 4521.235);
    *worker1 = "Jane Doe";
    *factory += *worker1;
    *factory += *worker2;
    cout << *factory;
    delete factory;
    return 0;
}