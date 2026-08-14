#ifndef METADATA_H
#define METADATA_H

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

struct Date{
    int day;
    int month;
    int year;
};

inline string indent(int level) {
    string s;
    for (int i = 0; i < level; i++) 
        s += "  ";
    return s;
} 

inline bool isBefore(const Date& a, const Date& b) {
    if (a.year != b.year) 
        return a.year < b.year;
    if (a.month != b.month) 
        return a.month < b.month;
    return a.day < b.day;
}

enum Permission {read, write, full};

class Metadata{

    private:
        int size;
        string description;
        Date dateCreated;
        Date lastModified;
        Permission permission;

    public:
        Metadata();
        Metadata(int size, const string& description, Date dateCreated);
        ~Metadata();

        int getSize() const;
        const string& getDescription() const;
        Date getDateCreated() const;
        Date getDateModified() const;
        Permission getPermission() const;

        bool setSize(int size);
        void setDateModified(Date lastModified);
        void setDescription(const string& description);
        void setPermission(Permission permission);

        inline string toString(int level) const {
            ostringstream out;
            out << indent(level)     << "Metadata:\n";
            out << indent(level + 1) << "size:        " << size << "\n";
            out << indent(level + 1) << "description: " << description << "\n";
            out << indent(level + 1) << "created:     " << setfill('0') << setw(2) << dateCreated.day << "/" << setw(2) << dateCreated.month << "/" << setw(4) << dateCreated.year << "\n";
            out << indent(level + 1) << "modified:    " << setfill('0') << setw(2) << lastModified.day << "/" << setw(2) << lastModified.month << "/" << setw(4) << lastModified.year << "\n";  
            out << indent(level + 1) << "permission:  " << (permission == read ? "read-only" : permission == write ? "write-only" : "full") << "\n";
            return out.str();
        }
};

#endif //METADATA_H