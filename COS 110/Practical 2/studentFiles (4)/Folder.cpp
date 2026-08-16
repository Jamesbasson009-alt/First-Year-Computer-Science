#include "Folder.h"

Folder::Folder()
{
    name = "Untitled";
    metadata = Metadata();
    files = NULL;
    subFolders = NULL;
    totalFiles = 0;
    numFolders = 0;
    numFiles = 0;
}

Folder::Folder(const string &name, const string &description, Date dateCreated, File *&inputFiles, int count)
{
    this->name = name;
    this->metadata = Metadata(0, description, dateCreated);
    this->subFolders = NULL;
    int totalSize = 0;
    this->numFolders = 0;

    if (count > 0 && inputFiles != NULL)
    {
        this->numFiles = count;
        this->totalFiles = count;
        files = new File *[count];

        for (int i = 0; i < count; i++)
        {
            files[i] = new File(inputFiles[i]);
            totalSize += this->files[i]->getMetadata().getSize();
        }

        this->metadata.setSize(totalSize);
    }
    else if (count == 0 || inputFiles == NULL)
    {
        this->numFiles = 0;
        this->numFolders = 0;
        this->totalFiles = 0;
        this->files = NULL;
    }
}

Folder::Folder(const Folder &other)
{
    this->name = other.name;
    this->metadata = other.metadata;
    this->numFiles = other.numFiles;
    this->numFolders = other.numFolders;
    this->totalFiles = other.totalFiles;

    if (other.numFiles > 0)
    {
        files = new File *[other.numFiles];
        for (int i = 0; i < other.numFiles; i++)
        {
            files[i] = new File(*other.files[i]);
        }
    }
    else
    {
        files = NULL;
    }
    if (other.numFolders > 0)
    {
        subFolders = new Folder *[other.numFolders];
        for (int i = 0; i < other.numFolders; i++)
        {
            subFolders[i] = new Folder(*other.subFolders[i]);
        }
    } else {
        subFolders = NULL;
    }
}

const string& Folder::getName() const {
    return this->name;
}

const Metadata& Folder::getMetadata() const {
    return this->metadata;
}

int Folder::getNumFiles() const {
    return this->numFiles;
}

int Folder::getNumFolders() const {
    return this->numFolders;
}

int Folder::getTotalFiles() const {
    return this->totalFiles;
}

void Folder::addFile(const File& file, Date lastModified) {
    File** newArr = new File*[numFiles + 1];
    for (int i = 0; i < numFiles; i++) {
        newArr[i] = files[i];
    }
    newArr[numFiles] = new File(file);

    delete[] files;
    files = newArr;

    numFiles++;
    totalFiles++;
    metadata.setSize(metadata.getSize() + file.getMetadata().getSize());
    metadata.setDateModified(lastModified);
}

void Folder::addFolder(const Folder& folder, Date lastModified) {
    Folder** newArr = new Folder*[numFolders + 1];
    for (int i = 0; i < numFolders; i++) {
        newArr[i] = subFolders[i];
    }
    newArr[numFolders] = new Folder(folder);

    delete[] subFolders;
    subFolders = newArr;

    numFolders++;
    totalFiles += folder.getTotalFiles();
    metadata.setSize(metadata.getSize() + folder.getMetadata().getSize());
    metadata.setDateModified(lastModified);
}
    
Folder::~Folder() {
    
    for (int i = 0; i < numFiles; i++) {
        delete files[i];
    }
    delete[] files;

    for (int i = 0; i < numFolders; i++) {
        delete subFolders[i];
    }
    delete[] subFolders;

}
