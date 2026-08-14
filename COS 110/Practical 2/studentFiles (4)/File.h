#ifndef FILE_H
#define FILE_H

#include "Metadata.h"

enum Extension {txt, csv, mp3};

inline string extensionToString(Extension ext) {
    return ext == txt ? "txt" : ext == csv ? "csv" : "mp3";
}

class File{

    private:
        string name;
        Extension ext;
        Metadata metadata;
        int dataSize;
        int* data;

        void copyDataFrom(const File& other);
        void allocateFile(int dataSize);
        void clearFile();
    
    public:
        File();
        File(const string& name, Extension ext, const string& description, Date dateCreated);
        File(const File& other);
        ~File();

        void setName(const string& name);
        void setExt(Extension ext);

        const string& getName() const;
        Extension getExt() const;
        const Metadata& getMetadata() const;
        int getDataSize() const;

        string fullName() const;
        bool writeToFile(int size, Date lastModified);

        inline string toString(int level) const {
            ostringstream out;
            out << indent(level) << "File: " << fullName()
                << "  [" << extensionToString(ext) << "]\n";

            out << metadata.toString(level + 1);
            out << indent(level + 1) << "Contents: ";
            if (data == NULL) {
                out << "(empty)\n";
            } else {
                out << "(" << dataSize << " values)\n" << indent(level + 2);
                for (int i = 0; i < dataSize; i++) {
                    out << data[i];
                    if (i + 1 < dataSize) out << " ";
                }
                out << "\n";
            }

            return out.str();
        }

};

#endif //FILE_H