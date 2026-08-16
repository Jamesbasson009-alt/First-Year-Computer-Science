#include "File.h"

File::File() {
    name = "Untitled";
    ext = txt;
    metadata = Metadata();
    dataSize = 0;
    data = NULL;
}

File::File(const string& name, Extension ext, const string& description, Date dateCreated) {

    this->name = name;
    this->ext = ext;
    this->metadata = Metadata(0, description, dateCreated);
    this->dataSize = 0;
    this->data = NULL;

}

File::File(const File& other) {

    this->name = other.name;
    this->ext = other.ext;
    this->metadata = other.metadata;
    this->dataSize = 0;
    this->data = NULL;

    copyDataFrom(other);

}

void File::copyDataFrom(const File& other) {

    if (other.data == NULL) {
        return;
    }

    this->dataSize = other.dataSize;
    this->data = new int[dataSize];

    for(int i = 0; i < other.dataSize; i++) {

        this->data[i] = other.data[i];

    }
    
}

void File::clearFile() {

    delete[] data;
    data = NULL;
    dataSize = 0;
    metadata.setSize(0);

}

void File::allocateFile(int dataSize) {

    delete[] data;
    data = NULL;
    data = new int[dataSize];
    for (int i = 0; i < dataSize; i++) {
        data[i] = 1;
    }
    this->dataSize = dataSize;

}

bool File::writeToFile(int size, Date lastModified) {
    if (this->metadata.getPermission() == read || size <= 0) {
        return false;
    }
    allocateFile(size);
    this->metadata.setSize(size * sizeof(int));
    this->metadata.setDateModified(lastModified);
    return true;
}

void File::setName(const string& name) {
    this->name = name;
}

void File::setExt(Extension ext) {
    this->ext = ext;
}

const string& File::getName() const {
    return this->name;
}

Extension File::getExt() const {
    return this->ext;
}

const Metadata& File::getMetadata() const {
    return this->metadata;
}

int File::getDataSize() const {
    return this->dataSize;
}

std::string File::fullName() const {
    return this->name + "." + extensionToString(this->ext);
}

File::~File() {
    delete[] data;
    data = NULL;
}
    


    
    
    
    



