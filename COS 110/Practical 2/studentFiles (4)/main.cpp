#include <iostream>

#include "Folder.h"
 
int main() {
    Date jan = {15, 1, 2026};
    Date aug = {10, 8, 2026};
 
    File docFiles[2] = { File("notes", txt,   "meeting notes", jan), File("data",  csv, "results grid",  aug)};
    File* docPtr = docFiles;

    Folder docs("Documents", "My personal documents", aug, docPtr, 2);
    cout << docs.toString(0) << endl;

    return 0;
}
 