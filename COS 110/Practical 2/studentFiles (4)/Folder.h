#ifndef FOLDER_H
#define FOLDER_H

#include <iostream>
#include "File.h"

class Folder{
    private:
        string name;
        Metadata metadata;

        File** files;
        Folder** subFolders;

        int numFiles;
        int numFolders;
        int totalFiles;

    public:
        Folder();
        Folder(const string& name,const string& description, Date dateCreated, File*& inputFiles, int count);
        Folder(const Folder& other);
        ~Folder();

        const string& getName() const;
        const Metadata& getMetadata() const;
        int getNumFiles() const;
        int getNumFolders() const;
        int getTotalFiles() const;

        void addFile(const File& file, Date lastModified);
        void addFolder(const Folder& file, Date lastModified);

        inline string toString(int level) const {
            ostringstream out;
            out << indent(level) << "Folder: " << name << "\n";
        
            out << indent(level + 1) << "Files:       " << numFiles << "\n";
            out << indent(level + 1) << "Sub-Folders: " << numFolders << "\n";
            out << indent(level + 1) << "Total files: " << totalFiles << "\n";

            out << metadata.toString(level + 1);
            out << indent(level + 1) << "Contents:    " << "\n";
            for (int i = 0; i < numFolders; i++) 
                out << subFolders[i]->toString(level + 2);
            for (int i = 0; i < numFiles; i++)   
                out << files[i]->toString(level + 2);
            return out.str();
        }

};

#endif //FOLDER_H