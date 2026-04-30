#include <iostream>
#include "StringLib.h"

namespace StringLib {
    void print(const char* cptr) {
        if (cptr == NULL) {
            std::cout << std::endl;
            return;
        }

        if (*cptr == '\0') {
            std::cout << std::endl;
            return;
        }

        std::cout << *cptr;
        print(cptr + 1);
    }

    int length(const char* cptr) {
        if (cptr == NULL || *cptr == '\0') {
            return 0;
        }

        return 1 + length(cptr + 1);
    }

    int occurances(const char* cptr, char c) {
        if (cptr == NULL || *cptr == '\0') {
            return 0;
        }

        if (*cptr == c) {
            return 1 + occurances(cptr + 1, c);
        }

        return occurances(cptr + 1, c);
    }

    const char* find(const char* cptr, char c, int n) {
        if (cptr == NULL || *cptr == '\0' || n <= 0) {
            return NULL;
        }

        if (*cptr == c) {
            if (n == 1) {
                return cptr;
            }

            return find(cptr + 1, c, n - 1);
        }

        return find(cptr + 1, c, n);
    }

    void reverseHelper(const char* cptr, char* rptr, int index) {
        if (index < 0) {
            *rptr = '\0';
            return;
        }

        *rptr = cptr[index];
        reverseHelper(cptr, rptr + 1, index - 1);
    }

    char* reverse(const char* cptr, char* rptr) {
        if (cptr == NULL || rptr == NULL) {
            return NULL;
        }

        reverseHelper(cptr, rptr, length(cptr) - 1);
        return rptr;
    }

    void copyOver(const char* cptr, char* rptr) {
        if (cptr == NULL || rptr == NULL) {
            return;
        }

        if (*cptr == '\0') {
            *rptr = '\0';
            return;
        }

        *rptr = *cptr;
        copyOver(cptr + 1, rptr + 1);
    }

    void concatenateSecond(const char* c2ptr, char* result) {
        if (*c2ptr == '\0') {
            *result = '\0';
            return;
        }

        *result = *c2ptr;
        concatenateSecond(c2ptr + 1, result + 1);
    }

    void concatenateFirst(const char* c1ptr, const char* c2ptr, char* result) {
        if (*c1ptr == '\0') {
            concatenateSecond(c2ptr, result);
            return;
        }

        *result = *c1ptr;
        concatenateFirst(c1ptr + 1, c2ptr, result + 1);
    }

    void concatenate(const char* c1ptr, const char* c2ptr, char* result) {
        if (c1ptr == NULL || c2ptr == NULL || result == NULL) {
            return;
        }

        concatenateFirst(c1ptr, c2ptr, result);
    }

    void replace(char* cptr, char u, char r) {
        if (cptr == NULL || *cptr == '\0') {
            return;
        }

        if (*cptr == u) {
            *cptr = r;
        }

        replace(cptr + 1, u, r);
    }

    void setTo(char* cptr, char c) {
        if (cptr == NULL || *cptr == '\0') {
            return;
        }

        *cptr = c;
        setTo(cptr + 1, c);
    }

    char* findSmallest(char* cptr, char currentSmallest) {
        if (cptr == NULL || *cptr == '\0') {
            return NULL;
        }

        char* smallestInRest = findSmallest(cptr + 1, currentSmallest);

        if (smallestInRest == NULL) {
            return cptr;
        }

        if (*cptr <= *smallestInRest) {
            return cptr;
        }

        return smallestInRest;
    }

    char* findLargest(char* cptr, char currentLargest) {
        if (cptr == NULL || *cptr == '\0') {
            return NULL;
        }

        char* largestInRest = findLargest(cptr + 1, currentLargest);

        if (largestInRest == NULL) {
            return cptr;
        }

        if (*cptr >= *largestInRest) {
            return cptr;
        }

        return largestInRest;
    }

    char* findSmallestActive(char* cptr) {
        if (cptr == NULL || *cptr == '\0') {
            return NULL;
        }

        char* smallestInRest = findSmallestActive(cptr + 1);

        if (*cptr == 127) {
            return smallestInRest;
        }

        if (smallestInRest == NULL) {
            return cptr;
        }

        if (*cptr <= *smallestInRest) {
            return cptr;
        }

        return smallestInRest;
    }

    char* findLargestActive(char* cptr) {
        if (cptr == NULL || *cptr == '\0') {
            return NULL;
        }

        char* largestInRest = findLargestActive(cptr + 1);

        if (*cptr == 1) {
            return largestInRest;
        }

        if (largestInRest == NULL) {
            return cptr;
        }

        if (*cptr >= *largestInRest) {
            return cptr;
        }

        return largestInRest;
    }

    void sortAscendingHelper(char* cptr, char* rptr) {
        char* smallest = findSmallestActive(cptr);

        if (smallest == NULL) {
            *rptr = '\0';
            return;
        }

        *rptr = *smallest;
        *smallest = 127;

        sortAscendingHelper(cptr, rptr + 1);
    }

    void sortAscending(char* cptr, char* rptr) {
        if (cptr == NULL || rptr == NULL) {
            return;
        }

        sortAscendingHelper(cptr, rptr);
    }

    void sortDescendingHelper(char* cptr, char* rptr) {
        char* largest = findLargestActive(cptr);

        if (largest == NULL) {
            *rptr = '\0';
            return;
        }

        *rptr = *largest;
        *largest = 1;

        sortDescendingHelper(cptr, rptr + 1);
    }

    void sortDescending(char* cptr, char* rptr) {
        if (cptr == NULL || rptr == NULL) {
            return;
        }

        sortDescendingHelper(cptr, rptr);
    }
}