#include "Worker.h"

Worker::Worker(std::string name, double salary) {
    this->name = name;
    this->salary = salary;
    ID = idCounter;
    idCounter++;
}

Worker& Worker::operator=(double salary) {
    this->salary = salary;
    return *this;
}

Worker& Worker::operator=(std::string name) {
    this->name = name;
    return *this;
}

Worker& Worker::operator+=(int increase) {
    salary += increase;
    return *this;
}

Worker& Worker::operator-=(int decrease) {
    salary -= decrease;
    return *this;
}

bool Worker::operator==(const Worker& other) {
    if (this->name == other.name && this->salary == other.salary) {
        return true;
    }
    return false;
}

bool Worker::operator!=(const Worker& other) {
    return !(*this == other);
}

