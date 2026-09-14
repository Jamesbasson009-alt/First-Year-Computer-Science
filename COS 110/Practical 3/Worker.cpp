#include "Worker.h"
#include <iomanip>

int Worker::idCounter = 1;


Worker::Worker(std::string name, double salary) {
    this->name = name;
    this->salary = salary;
    ID = idCounter;
    idCounter++;
}

Worker& Worker::operator=(double salary) {
    if (salary < 0) {
        return *this;
    }
    this->salary = salary;
    return *this;
}

Worker& Worker::operator=(std::string name) {
    if (name.empty()) {
        return *this;
    }
    this->name = name;
    return *this;
}

Worker& Worker::operator+=(int increase) {
    if (increase < 0) {
        return *this;
    }
    salary += increase;
    return *this;
}

Worker& Worker::operator-=(int decrease) {
    if (decrease < 0 || salary - decrease < 0) {
        return *this;
    }
    salary -= decrease;
    return *this;
}

bool Worker::operator==(const Worker& other) const {
    if (this->name == other.name && this->salary == other.salary) {
        return true;
    }
    return false;
}

bool Worker::operator!=(const Worker& other) const {
    return !(*this == other);
}

bool Worker::operator<(const Worker& other) const {
    return this->salary < other.salary;
}

bool Worker::operator>(const Worker& other) const {
    return this->salary > other.salary;
}

bool Worker::operator<=(const Worker& other) const {
    return this->salary <= other.salary;
}

bool Worker::operator>=(const Worker& other) const {
    return this->salary >= other.salary;
}

Worker& Worker::operator=(const Worker& other) {
    if (this == &other) {
        return *this;
    }
    this->name = other.name;
    this->salary = other.salary;
    return *this;
}

double Worker::operator()() const {
    return salary;
}



std::ostream& operator<<(std::ostream& os, const Worker& worker) {
    os << "Worker: " << worker.ID << "," << worker.name << ",R"
       << std::fixed << std::setprecision(2) << worker.salary;
    return os;
}

Worker::~Worker() {

}
