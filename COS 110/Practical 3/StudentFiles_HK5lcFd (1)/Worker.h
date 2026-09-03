#include <string>
#include <ostream>
#ifndef WORKER_H
#define WORKER_H

class Worker {
    static int idCounter;
    private:
    int ID;
    std::string name;
    double salary;

    public:  
    Worker(std::string name, double salary);
    ~Worker();
    Worker& operator=(std::string name);
    Worker& operator=(double salary);
    Worker& operator+=(int increase);
    Worker& operator-=(int decerease);
    bool operator==(const Worker& other);
    bool operator!=(const Worker& other);
    bool operator>(const Worker& other) const;
    bool operator<(const Worker& other) const;
    bool operator>=(const Worker& other) const;
    bool operator<=(const Worker& other) const;
    Worker& operator=(const Worker& other);
    double operator()();
    friend std::ostream& operator<<(std::ostream& os, const Worker& worker);
};

#endif