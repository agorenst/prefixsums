#include "tree_array.h"

/*
 * This computes the "prefix sums" of a given array.
 * With infinite processors and other somesuch dreams,
 * it runs in logn time with linear work, which is optimal
 * 
 * This does run very nicely, better than other implementations.
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
/*
template<typename T>
void cont_prefix_sums(T* a, int n) {
    cont_tree_array<T> A(a, n);

    for (int h = 1; (1 << h) <= n; ++h) {
        for (int i = 0; i < n / (1 << h); ++i) {
            A(h,i) =  A(h-1,2*i+1) + A(h-1, 2*i);
        }
    }

    for (int h = A.height; h >= 0; --h) {
        for (int i = 0; i < n / (1 << h); ++i) {
            if (i == 0) {
            } else if (i % 2 == 1) {
                A(h,i) = A(h+1,(i-1)/2);
            } else {
                A(h,i) = A(h+1,(i/2)-1) + A(h,i);
            }
        }
    }
}

template<typename dataIter, typename outIter, typename T>
void contiguous_prefix_sums(dataIter begin, dataIter end, outIter output) {
    tree_array<T> B(begin, end);
    int n = B.n;

    // this is the first half of the algorithm (with recursion unrolled).
    // for h = 0 (the leaves of B), we've already initialized them.
    for (int h = 1; (1 << h) <= n; ++h) {
        // this is the first loop in the psuedocode.
        // the [h-1] index is the recursion count, and the second index is x_{2i} etc.
        for (int i = 0; i < n / (1 << h); ++i) {
            B[h][i] = B[h-1][2*i+1] + B[h-1][2*i];
        }
    }

    // now we do the second loop.
    for (int h = B.height; h >= 0; --h) {
        for (int i = 0; i < n / (1<<h); ++i) {
            if (i == 0) {
            } else if (i % 2 == 1) {
                B[h][i] = B[h+1][(i-1)/2];
            } else {
                B[h][i] = B[h+1][(i/2)-1] + B[h][i];
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        *output = B[0][i];
        ++output;
    }
}

template<typename dataIter, typename outIter, typename T>
void smaller_prefix_sums(dataIter begin, dataIter end, outIter output) {
    tree_array<T> B(begin, end);
    int n = B.n;

    // this is the first half of the algorithm (with recursion unrolled).
    // for h = 0 (the leaves of B), we've already initialized them.
    for (int h = 1; (1 << h) <= n; ++h) {
        // this is the first loop in the psuedocode.
        // the [h-1] index is the recursion count, and the second index is x_{2i} etc.
#pragma omp parallel for
        for (int i = 0; i < n / (1 << h); ++i) {
            B[h][i] = B[h-1][2*i+1] + B[h-1][2*i];
        }
    }

    // now we do the second loop.
    for (int h = B.height; h >= 0; --h) {
#pragma omp parallel for
        for (int i = 0; i < n / (1<<h); ++i) {
            if (i == 0) {
            } else if (i % 2 == 1) {
                B[h][i] = B[h+1][(i-1)/2];
            } else {
                B[h][i] = B[h+1][(i/2)-1] + B[h][i];
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        *output = B[0][i];
        ++output;
    }
}

template<typename dataIter, typename outIter, typename T>
void prefix_sums(dataIter begin, dataIter end, outIter output) {

    tree_array<T> B(begin, end);
    int n = B.n;

    // the first pass, build the tree up
    int h;
    for (h = 1; (1<<h) <= n; ++h) {
#pragma omp parallel for
        for (int i = 0; i < n / (1<<h); ++i) {
            B[h][i] = B[h-1][2*i+1] + B[h-1][2*i];
        }
    }

    // the second pass, build shit down (why do we need a second tree_array?
    tree_array<T> C(begin, end);
    for (; h >= 0; --h) {
#pragma omp parallel for
        for (int i = 0; i < n / (1<<h); ++i) {
            if (i == 0) {
                C[h][i] = B[h][i];
            } else if (i % 2 == 1) {
                C[h][i] = C[h+1][(i-1)/2];
            } else {
                C[h][i] = C[h+1][(i/2)-1] + B[h][i];
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        *output = C[0][i];
        ++output;
    }
}
*/
