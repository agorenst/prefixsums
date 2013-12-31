#ifndef _PAR_H_
#define _PAR_H_

#include "par_utils.h"

/*
 * This is the "first half" of the parallel-prefix-sums, the
 * pointer jumping. Used in MST to collapse trees into a single
 * "vertex representative". O(lgn)
 */
template<class T>
void pointer_jumping(const T P[], T S[], const int size) {
  copy_array(P, S, size);
  #pragma omp parallel for
  for (int i = 0; i < size; ++i) {
    while (S[i] != S[S[i]]) {
      S[i] = S[S[i]];
    }
  }
}

// this may not work, because of issues with updating W[i] and W[S[i]]
// in proper time......
// requires coordination, blah... :/
template<class T, class R>
void parallel_prefix(const T P[], T S[], R W[], const int size) {
  copy_array(P, S, size);
  #pragma omp parallel for
  for (int i = 0; i < size; ++i) {
    while (S[i] != S[S[i]]) {
      W[i] += W[S[i]];
      S[i] = S[S[i]];
    }
  }  
}
template<class T, class R>
void serial_prefix(const T P[], T S[], R W[], const int size) {
  copy_array(P, S, size);
  for (int i = 0; i < size; ++i) {
    while (S[i] != S[S[i]]) {
      W[i] += W[S[i]];
      S[i] = S[S[i]];
    }
  }  
}

/*
 * This computes the "prefix sums" of a given array.
 * With infinite processors and other somesuch dreams,
 * it runs in logn time with linear work.
 * 
 * Due to its good work balance, it in fact does run
 * quickly even on this computer's first-gen atom processor,
 * with hyperthreading.
 *
 * To do: parameterize the + function, so it can take any
 * appropriate operator.
 * Maybe also improve the related data structure..
 */
template<class T>
void prefix_sums(const T A[], T D[], const int n) {
  tree_array<T> B(n);
  for (int i = 0; i < n; ++i) {
    B[0][i] = A[i];
  }
  int h;
  for (h = 1; (1<<h) <= n; ++h) {
    #pragma omp parallel for
    for (int i = 0; i < n / (1<<h); ++i) {
      B[h][i] = B[h-1][2*i+1] + B[h-1][2*i];
    }
  }

  tree_array<T> C(n);
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
  C.copy_bottom(D);
}

template<class T>
inline void swap(T& a, T& b) {
  T t = a;
  a = b;
  b = t;
}

/*
 * Not really in-place, and due to locality issues (I think)
 * it is a lot slower. Oh well. :(
 */
template<class T>
void in_place_prefix_sums(T A[], T B[], const int n) {
  for (int i = 1; int l = (1 << (i-1)) < n; ++i) {
    #pragma omp parallel for
    for (int j = l; j < n; ++j) {
      B[j] = A[j] + A[j-l];
    }
    swap(A, B);
  }
}

template<class T>
void new_in_place(sync_array<T> A) {
  const int n = A.n;
  for (int i = 1; int l = (1 << (i-1)) < n; ++i) {
    for (int j = l; j < n; ++j) {
      A[j] = A[j] + A[j-l];
    }
    A.resync();
  }
}

/*
 * Counts the number of elements less than or equal to k
 * in list A. Note that we overload the fact that true == 1.
 * array_cmp is O(1), prefix_sums is O(logn), we're all set.
 */
template<class T>
int rank(const T A[], const T k, const int size) {
  int B[size];
  array_cmp(A, B, k, size, int_lte);
  return sum_reduction(B, size);
}

template<class T>
void rank(const T A[], const T B[], int R[], const int size_a, const int size_b) {
  #pragma omp parallel for
  for (int i = 0; i < size_b; ++i) {
    R[i] = rank(A, B[i], size_a);
  }
}

#endif
