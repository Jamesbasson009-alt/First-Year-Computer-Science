#ifndef PRAC_STRINGLIB_H
#define PRAC_STRINGLIB_H

namespace StringLib {
    void print(const char* cptr);
    int length(const char* cptr);
    int occurances(const char* cptr, char c);
    const char* find(const char* cptr, char c, int n);
    char* reverse(const char* cptr, char* rptr);
    void copyOver(const char* cptr, char* rptr);
    void concatenate(const char* c1ptr, const char* c2ptr, char* result);
    void replace(char* cptr, char u, char r);
    void setTo(char* cptr, char c);
    char* findSmallest(char* cptr, char currentSmallest = 127);
    char* findLargest(char* cptr, char currentLargest = 1);
    void sortAscending(char* cptr, char* rptr);
    void sortDescending(char* cptr, char* rptr);

}

#endif
