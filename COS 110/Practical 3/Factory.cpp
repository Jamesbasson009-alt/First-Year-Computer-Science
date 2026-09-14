#include "Factory.h"
#include <fstream>
#include <sstream>
#include <string>

namespace
{

    bool parseWorkerLine(const std::string &line, std::string &name, double &salary)
    {
        std::string::size_type colon = line.find(':');
        if (colon == std::string::npos)
        {
            return false;
        }

        std::string rest = line.substr(colon + 1);

        std::string::size_type firstComma = rest.find(',');
        if (firstComma == std::string::npos)
        {
            return false;
        }

        std::string::size_type secondComma = rest.find(',', firstComma + 1);
        if (secondComma == std::string::npos)
        {
            return false;
        }

        name = rest.substr(firstComma + 1, secondComma - firstComma - 1);

        std::string salaryPart = rest.substr(secondComma + 1);
        std::string::size_type rPos = salaryPart.find('R');
        if (rPos != std::string::npos)
        {
            salaryPart = salaryPart.substr(rPos + 1);
        }

        std::istringstream iss(salaryPart);
        iss >> salary;

        return true;
    }

    std::string extractWorkerName(const Worker &worker)
    {
        std::ostringstream oss;
        oss << worker;
        std::string name;
        double salary = 0.0;
        parseWorkerLine(oss.str(), name, salary);
        return name;
    }

}

Factory::Factory() : workers(0), numWorkers(0)
{
}

Factory::~Factory()
{
    for (int i = 0; i < numWorkers; ++i)
    {
        delete workers[i];
    }
    delete[] workers;
}

Factory &Factory::operator=(const Factory &other)
{
    if (this == &other)
    {
        return *this;
    }

    for (int i = 0; i < numWorkers; ++i)
    {
        delete workers[i];
    }
    delete[] workers;

    numWorkers = other.numWorkers;
    workers = (numWorkers > 0) ? new Worker *[numWorkers] : 0;
    for (int i = 0; i < numWorkers; ++i)
    {
        workers[i] = new Worker(*other.workers[i]);
    }

    return *this;
}

Factory &Factory::operator|(std::string filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        return *this;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::string name;
        double salary = 0.0;
        if (parseWorkerLine(line, name, salary))
        {
            Worker *newWorker = new Worker(name, salary);

            Worker **newArray = new Worker *[numWorkers + 1];
            for (int i = 0; i < numWorkers; ++i)
            {
                newArray[i] = workers[i];
            }
            newArray[numWorkers] = newWorker;

            delete[] workers;
            workers = newArray;
            ++numWorkers;
        }
    }

    file.close();
    return *this;
}

int Factory::operator[](const Worker &worker) const
{
    for (int i = 0; i < numWorkers; ++i)
    {
        if (*workers[i] == worker)
        {
            return i;
        }
    }
    return -1;
}

Worker *Factory::operator[](int index) const
{
    if (index < 0 || index >= numWorkers)
    {
        return 0;
    }
    return workers[index];
}

void Factory::operator*=(const Factory &other)
{
    for (int i = 0; i < other.numWorkers; ++i)
    {
        (*this) += other.workers[i];
    }
}

Factory *Factory::operator*(const Factory &other) const
{
    Factory *result = new Factory();

    for (int i = 0; i < numWorkers; ++i)
    {
        (*result) += workers[i];
    }
    for (int i = 0; i < other.numWorkers; ++i)
    {
        (*result) += other.workers[i];
    }

    return result;
}

Factory &Factory::operator+=(Worker *worker)
{
    if (worker == 0)
    {
        return *this;
    }

    for (int i = 0; i < numWorkers; ++i)
    {
        if (*workers[i] == *worker)
        {
            return *this;
        }
    }

    std::string name = extractWorkerName(*worker);
    double salary = (*worker)();
    Worker *copy = new Worker(name, salary);

    Worker **newArray = new Worker *[numWorkers + 1];
    for (int i = 0; i < numWorkers; ++i)
    {
        newArray[i] = workers[i];
    }
    newArray[numWorkers] = copy;

    delete[] workers;
    workers = newArray;
    ++numWorkers;

    return *this;
}

Factory &Factory::operator+=(Worker &worker)
{
    for (int i = 0; i < numWorkers; ++i)
    {
        if (*workers[i] == worker)
        {
            return *this;
        }
    }

    Worker **newArray = new Worker *[numWorkers + 1];
    for (int i = 0; i < numWorkers; ++i)
    {
        newArray[i] = workers[i];
    }
    newArray[numWorkers] = &worker;

    delete[] workers;
    workers = newArray;
    ++numWorkers;

    return *this;
}

Factory &Factory::operator-=(Worker *worker)
{
    if (worker == 0)
    {
        return *this;
    }

    int index = (*this)[*worker];
    if (index == -1)
    {
        return *this;
    }

    delete workers[index];

    Worker **newArray = (numWorkers - 1 > 0) ? new Worker *[numWorkers - 1] : 0;
    int j = 0;
    for (int i = 0; i < numWorkers; ++i)
    {
        if (i != index)
        {
            newArray[j] = workers[i];
            ++j;
        }
    }

    delete[] workers;
    workers = newArray;
    --numWorkers;

    return *this;
}

bool Factory::operator==(const Factory &other) const
{
    double total1 = 0.0;
    double total2 = 0.0;
    for (int i = 0; i < numWorkers; ++i)
    {
        total1 += (*workers[i])();
    }
    for (int i = 0; i < other.numWorkers; ++i)
    {
        total2 += (*other.workers[i])();
    }
    return total1 == total2;
}

bool Factory::operator!=(const Factory &other) const
{
    return !(*this == other);
}

const Factory& Factory::operator>(const Factory& other) const {
    static Factory* equalFactory = new Factory();

    double total1 = 0.0;
    double total2 = 0.0;
    for (int i = 0; i < numWorkers; ++i) {
        total1 += (*workers[i])();
    }
    for (int i = 0; i < other.numWorkers; ++i) {
        total2 += (*other.workers[i])();
    }

    if (total1 == total2) {
        return *equalFactory;
    }
    return (total1 > total2) ? *this : other;
}

const Factory& Factory::operator<(const Factory& other) const {
    static Factory* equalFactory = new Factory();

    double total1 = 0.0;
    double total2 = 0.0;
    for (int i = 0; i < numWorkers; ++i) {
        total1 += (*workers[i])();
    }
    for (int i = 0; i < other.numWorkers; ++i) {
        total2 += (*other.workers[i])();
    }

    if (total1 == total2) {
        return *equalFactory;
    }
    return (total1 < total2) ? *this : other;
}

const Factory& Factory::operator>=(const Factory& other) const {
    static Factory* equalFactory = new Factory();

    double total1 = 0.0;
    double total2 = 0.0;
    for (int i = 0; i < numWorkers; ++i) {
        total1 += (*workers[i])();
    }
    for (int i = 0; i < other.numWorkers; ++i) {
        total2 += (*other.workers[i])();
    }

    if (total1 == total2) {
        return *equalFactory;
    }
    return (total1 >= total2) ? *this : other;
}

const Factory& Factory::operator<=(const Factory& other) const {
    static Factory* equalFactory = new Factory();

    double total1 = 0.0;
    double total2 = 0.0;
    for (int i = 0; i < numWorkers; ++i) {
        total1 += (*workers[i])();
    }
    for (int i = 0; i < other.numWorkers; ++i) {
        total2 += (*other.workers[i])();
    }

    if (total1 == total2) {
        return *equalFactory;
    }
    return (total1 <= total2) ? *this : other;
}

std::ostream &operator<<(std::ostream &os, const Factory &factory)
{
    if (factory.numWorkers == 0)
    {
        os << "Empty factory";
    }
    else
    {
        for (int i = 0; i < factory.numWorkers; ++i)
        {
            os << *factory.workers[i] << "\n";
        }
    }
    return os;
}

std::istream &operator>>(std::istream &is, Factory &factory)
{
    std::string line;
    while (std::getline(is, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::string name;
        double salary = 0.0;
        if (parseWorkerLine(line, name, salary))
        {
            Worker *newWorker = new Worker(name, salary);

            Worker **newArray = new Worker *[factory.numWorkers + 1];
            for (int i = 0; i < factory.numWorkers; ++i)
            {
                newArray[i] = factory.workers[i];
            }
            newArray[factory.numWorkers] = newWorker;

            delete[] factory.workers;
            factory.workers = newArray;
            ++factory.numWorkers;
        }
    }
    return is;
}