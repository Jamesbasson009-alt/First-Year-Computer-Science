#include <iostream>
#include <cassert>
#include "Metadata.h"
#include "File.h"
#include "Folder.h"

using namespace std;

static int testsRun = 0;
static int testsPassed = 0;

void check(bool condition, const string& description) {
    testsRun++;
    if (condition) {
        testsPassed++;
        cout << "  [PASS] " << description << "\n";
    } else {
        cout << "  [FAIL] " << description << "\n";
    }
}

void section(const string& title) {
    cout << "\n===== " << title << " =====\n";
}

void testMetadata() {
    section("Metadata");

    Metadata m1;
    check(m1.getSize() == 0, "default ctor: size == 0");
    check(m1.getDescription() == "New file/folder", "default ctor: description");
    Date d1 = m1.getDateCreated();
    check(d1.day == 10 && d1.month == 8 && d1.year == 2026, "default ctor: dateCreated == 10/8/2026");
    check(m1.getPermission() == full, "default ctor: permission == full");

    Date created{1, 1, 2025};
    Metadata m2(500, "Important document", created);
    check(m2.getSize() == 500, "param ctor: size == 500");
    check(m2.getDescription() == "Important document", "param ctor: description");
    Date d2 = m2.getDateCreated();
    check(d2.day == 1 && d2.month == 1 && d2.year == 2025, "param ctor: dateCreated set correctly");
    Date d2mod = m2.getDateModified();
    check(d2mod.day == 1 && d2mod.month == 1 && d2mod.year == 2025, "param ctor: lastModified == dateCreated initially");
    check(m2.getPermission() == full, "param ctor: permission == full");

    check(m2.setSize(1000) == true, "setSize(1000) returns true");
    check(m2.getSize() == 1000, "setSize(1000) actually updates size");
    check(m2.setSize(-5) == false, "setSize(-5) returns false (rejected)");
    check(m2.getSize() == 1000, "setSize(-5) did not change size");

    m2.setDescription("Updated description");
    check(m2.getDescription() == "Updated description", "setDescription updates value");

    m2.setPermission(read);
    check(m2.getPermission() == read, "setPermission(read) updates value");
    m2.setPermission(write);
    check(m2.getPermission() == write, "setPermission(write) updates value");

    m2.setPermission(full);
    Metadata m3(10, "test", Date{15, 6, 2025});
    m3.setDateModified(Date{1, 6, 2025});
    Date after1 = m3.getDateModified();
    check(after1.day == 15 && after1.month == 6 && after1.year == 2025,
          "setDateModified rejects a date earlier than dateCreated");

    m3.setDateModified(Date{20, 6, 2025});
    Date after2 = m3.getDateModified();
    check(after2.day == 20 && after2.month == 6 && after2.year == 2025,
          "setDateModified accepts a valid later date");

    m3.setDateModified(Date{18, 6, 2025});
    Date after3 = m3.getDateModified();
    check(after3.day == 20 && after3.month == 6 && after3.year == 2025,
          "setDateModified rejects a date earlier than current lastModified");

    cout << "\n" << m2.toString(1);
}

void testFile() {
    section("File");

    File f1;
    check(f1.getName() == "Untitled", "default ctor: name == Untitled");
    check(f1.getExt() == txt, "default ctor: ext == txt");
    check(f1.getDataSize() == 0, "default ctor: dataSize == 0");
    check(f1.fullName() == "Untitled.txt", "default ctor: fullName()");

    File f2("Report", csv, "Sales report", Date{3, 3, 2026});
    check(f2.getName() == "Report", "param ctor: name");
    check(f2.getExt() == csv, "param ctor: ext == csv");
    check(f2.fullName() == "Report.csv", "param ctor: fullName()");
    check(f2.getMetadata().getSize() == 0, "param ctor: initial metadata size == 0");

    f2.setName("SalesReport");
    f2.setExt(mp3);
    check(f2.getName() == "SalesReport", "setName updates name");
    check(f2.getExt() == mp3, "setExt updates ext");
    check(f2.fullName() == "SalesReport.mp3", "fullName() reflects updated name/ext");

    bool ok = f2.writeToFile(5, Date{4, 3, 2026});
    check(ok == true, "writeToFile(5, ...) succeeds when permission == full");
    check(f2.getDataSize() == 5, "writeToFile sets dataSize == 5");
    check(f2.getMetadata().getSize() == (int)(5 * sizeof(int)), "writeToFile updates metadata size correctly");

    bool badSize = f2.writeToFile(0, Date{5, 3, 2026});
    check(badSize == false, "writeToFile(0, ...) rejected (size <= 0)");
    check(f2.getDataSize() == 5, "dataSize unchanged after rejected writeToFile");

    File f3("Locked", txt, "read only file", Date{1, 1, 2026});
    bool okDefaultPermission = f3.writeToFile(3, Date{2, 1, 2026});
    check(okDefaultPermission == true, "writeToFile succeeds by default (permission starts as full)");

    File f4(f2);
    check(f4.getName() == f2.getName(), "copy ctor: name copied");
    check(f4.getDataSize() == f2.getDataSize(), "copy ctor: dataSize copied");

    f2.writeToFile(9, Date{6, 3, 2026});
    check(f2.getDataSize() == 9, "original's dataSize changed after re-writing");
    check(f4.getDataSize() == 5, "copy's dataSize unaffected by change to original (deep copy)");

    cout << "\n" << f2.toString(1);
    cout << f4.toString(1);
}

void testFolder() {
    section("Folder");

    Folder folderEmpty;
    check(folderEmpty.getName() == "Untitled", "default ctor: name == Untitled");
    check(folderEmpty.getNumFiles() == 0, "default ctor: numFiles == 0");
    check(folderEmpty.getNumFolders() == 0, "default ctor: numFolders == 0");
    check(folderEmpty.getTotalFiles() == 0, "default ctor: totalFiles == 0");

    File* seedFiles = new File[2];
    seedFiles[0] = File("Notes", txt, "meeting notes", Date{1, 2, 2026});
    seedFiles[0].writeToFile(4, Date{1, 2, 2026});
    seedFiles[1] = File("Budget", csv, "budget sheet", Date{2, 2, 2026});
    seedFiles[1].writeToFile(10, Date{2, 2, 2026});

    Folder documents("Documents", "Personal documents", Date{1, 2, 2026}, seedFiles, 2);
    check(documents.getName() == "Documents", "param ctor: name");
    check(documents.getNumFiles() == 2, "param ctor: numFiles == 2");
    check(documents.getTotalFiles() == 2, "param ctor: totalFiles == 2");
    int expectedSize = (int)(4 * sizeof(int)) + (int)(10 * sizeof(int));
    check(documents.getMetadata().getSize() == expectedSize, "param ctor: folder size == sum of file sizes");

    delete[] seedFiles;

    File extra("Todo", txt, "todo list", Date{3, 2, 2026});
    extra.writeToFile(2, Date{3, 2, 2026});
    documents.addFile(extra, Date{4, 2, 2026});
    check(documents.getNumFiles() == 3, "addFile: numFiles increments to 3");
    check(documents.getTotalFiles() == 3, "addFile: totalFiles increments to 3");
    Date modAfterAdd = documents.getMetadata().getDateModified();
    check(modAfterAdd.day == 4 && modAfterAdd.month == 2 && modAfterAdd.year == 2026,
          "addFile: metadata lastModified updated");

    Folder photos("Photos", "Vacation photos", Date{5, 2, 2026}, seedFiles, 0);
    documents.addFolder(photos, Date{6, 2, 2026});
    check(documents.getNumFolders() == 1, "addFolder: numFolders increments to 1");
    check(documents.getTotalFiles() == 3, "addFolder: totalFiles unaffected by empty sub-folder");

    Folder documentsCopy(documents);
    check(documentsCopy.getNumFiles() == documents.getNumFiles(), "copy ctor: numFiles matches");
    check(documentsCopy.getNumFolders() == documents.getNumFolders(), "copy ctor: numFolders matches");

    File more("Scratch", txt, "scratch file", Date{7, 2, 2026});
    documents.addFile(more, Date{7, 2, 2026});
    check(documents.getNumFiles() == 4, "original: numFiles now 4 after further addFile");
    check(documentsCopy.getNumFiles() == 3, "copy: numFiles stays 3 (deep copy, unaffected by original)");

    cout << "\n" << documents.toString(0);
}

int main() {
    cout << "Running tests\n";

    testMetadata();
    testFile();
    testFolder();

    cout << "\n============================\n";
    cout << testsPassed << " / " << testsRun << " checks passed\n";
    cout << "============================\n";

    return (testsPassed == testsRun) ? 0 : 1;
}