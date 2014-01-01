#include <iostream>
using namespace std;
/*
 * This computes the "prefix sums" of a given array.
 * With infinite processors and other somesuch dreams,
 * it runs in logn time with linear work, which is optimal
 * * This does run very nicely, better than other implementations.
 *
 * To do: parameterize the + function, so it can take any
 * appropriate operator.
 * Maybe also improve the related data structure..
 */

/*
 * input: x_1 ... x_n
 * output: s_1 ... s_n
 * if n = 1 then s_i = x_1; exit}
 * for 1 <= i <= n/2 pardo
 *    set y_i: = x_{2i-1} * x_{2i}
 * recursively compute prefix sums {y_1,\ldots,y_{n/2}} and rename them as z_1,z_{n/2}
 * 
 * for 1 <= 1 <= n pardo
 *    i even     s_i = z_{i/2}
 *    i == 1     s_1 = x_1
 *    i odd > 1  s_i = z_{(i-1)/2}*x_i
 */


// this is also just for debugging
template<class T>
void print_array(const T A[], const int size, std::ostream& o = std::cout) {
    for (int i = 0; i < size; ++i) {
        o << A[i] << " ";
    }
    o << std::endl;
}


// this is just for debugging
template<typename T>
void serial_sums(T* a, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        a[i] = a[i]+a[i-1];
    }
}

/* Valgrind with --tool=drd reveals there are read/write conflicts,
 * but I am not sure if that's right.
 * I suspect it may be confused by the fact that the length of the
 * iteration changes, but everything is safe (parameterized by h)
 */
template<typename T>
void prefix_sums(T* a, size_t n) {
    // assume n is a power of two>1
    unsigned height = 0;
    while (unsigned((1 << height)) <= n) { ++height; } // we increment height 1 beyond the log

    // mem is the internal nodes of the binary tree, we know its size
    // This section defines the "tree", which we call STORE.
    T* mem = new T[n-1];
    T** store = new T*[height];
    store[0] = a; // leaves
    store[1] = mem; // assuming n > 1;
    for (unsigned i = 2; i < height; ++i) {
        unsigned offset = (1 << (height - i));
        store[i] = store[i-1]+offset;
    }


    // this is the first half of the algorithm (with recursion unrolled).
    // for h = 0 (the leaves of B), we've already initialized them.
    for (unsigned h = 1; unsigned(1 << h) <= n; ++h) {
        // this is the first loop in the psuedocode.
        // the [h-1] index is the recursion count, and the second index is x_{2i} etc.
#pragma omp parallel for
        for (unsigned i = 0; i < (n / (1 << h)); ++i) {
            store[h][i] = store[h-1][2*i+1] + store[h-1][2*i];
        }
    }

    // now we do the second loop.
    // if this is unsigned, something horrible happens (the test never fails!)
    for (int h = height-2; h >= 0; --h) {
#pragma omp parallel for
        for (unsigned i = 0; i < n / (1<<h); ++i) {
            if (i == 0) {
            } else if (i % 2 == 1) {
                store[h][i] = store[h+1][(i-1)/2];
            } else {
                store[h][i] = store[h+1][(i/2)-1] + store[h][i];
            }
        }
    }

    delete [] store;
    delete [] mem;

    // What's left? The original input array has been modified with the solution
}
