#ifndef _ARRAY_UTILS_
#define _ARRAY_UTILS_

#include <iostream>
#include <cstdlib>
#include <ctime>

template<class T>
void print_array(const T A[], const int size, std::ostream& o = std::cout) {
    for (int i = 0; i < size; ++i) {
        o << A[i] << " ";
    }
    o << std::endl;
}

void rand_array(int A[], const int len) {
    for (int i = 0; i < len; ++i) {
        A[i] = rand() % 100;
    }
}

void make_bad_par_prefix(int S[], int W[], const int len) {
#pragma omp parallel for
    for (int i = 0; i < len; ++i) {
        S[i] = i+1;
        W[i] = 1;
    }
    for (int i = 0; i < len; ++i) {
        if (rand() % 10 == 0) {
            S[i] = (rand() % (len-i)) + i;
        }
    }
    S[len-1] = len-1;
}

#endif
