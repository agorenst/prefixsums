// this are the not-clever algorithms, the straightfoward ones...

#ifndef _PAR_UTILS_H_
#define _PAR_UTILS_H_

/*
 * This data structure is designed to be used as a normal array,
 * but with 2 key differences:
 * first, it has its length built-in. Because every bloody thing
 * needs to know its length.
 * Secondly, it is an array of 2-tuples, one that is "Working"
 * and one that is "current".
 * Many parallel algorithms assume simultaneously actions, so
 * elminates race conditions (if A[i] = f(A[j]), it would work,
 * except for the fact that we really aren't doing everything simul.
 */
template<class T>
class sync_array {
    private:
        // simply contains 2 of the same element, which
        // is "working" and which is "current" varies.
        struct tuple {
            T a;
            T b;
        };
        tuple* A;
        bool a_readonly;
    public:
        const int n;
        sync_array(const T B[], const int len): a_readonly(true), n(len){
            A = new tuple[n];
            for (int i = 0; i < n; ++i) {
                A[i].a = B[i]; A[i]. b = B[i];
            }
        }
        ~sync_array() {
            delete A;
        }
        // write-in operator
        T& operator[](const int i) {
            return a_readonly ? A[i].b : A[i].a;
        }
        // read-only operator
        const T& operator[](int i) const {
            return a_readonly ? A[i].a : A[i].b;
        }
        void resync() {
            a_readonly = !a_readonly;
        }

};

// allows for random access
template<class T>
class tree_array {
    private:
        int k;
        int n;
    public:
        T** store;
        tree_array(const int n):n(n) {
            k = 0;
            while ((1<<k) < n) { ++k; } // find the log, basically.
            store = new T*[k+1];
            for (int h = 0; h <= k; ++h) {
                store[h] = new T[(1 << (k-h))];
            }
        }
        ~tree_array() {
            for (int h = 0; h <= k; ++h) {
                delete [] store[h];
            }
            delete [] store;
        }
        T* operator[](const int h) {
            return store[h];
        }
        void copy_bottom(T a[]) {
            for (int i = 0; i < n; ++i) {
                a[i] = store[0][i];
            }
        }
};

template<class T>
void copy_array(const T A[], T B[], const int len) {
#pragma omp parallel for
    for (int i = 0; i < len; ++i) {
        B[i] = A[i];
    }
}

template<class T>
bool equal_arrays(const T A[], const T B[], const int len) {
    bool to_return = true;
#pragma omp parallel for
    for (int i = 0; i < len; ++i) {
        if (A[i] != B[i]) { to_return = false; }
    }
    return to_return;
}


// this basically maps a boolean comparison A[i] op k into B[i].
// so there are the 6 standard comparisons, >, >=, <, <=, ==, and !=.
// if only I could pass functions as parameters. :(
template<class T>
bool lte(const T a, const T b) { return a <= b; }
template<class T>
bool lt(const T a, const T b) { return a < b; }
template<class T>
bool gte(const T a, const T b) { return a >= b; }
template<class T>
bool gt(const T a, const T b) { return a > b; }

template<class T>
int int_lte(const T a, const T b) { return a <= b ? 1 : 0; }

template<class T, class R>
void array_cmp(const T A[], R B[], const T k, const int len, R(*cmp)(const T, const T)) {
#pragma omp parallel for
    for (int i = 0; i < len; ++i) {
        B[i] = (*cmp)(A[i], k);
    }
}

void bool_to_int(const bool A[], int B[], const int size) {
#pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        B[i] = A[i] ? 1 : 0;
    }
}

int sum_reduction(const int A[], const int len) {
    int sum = 0;
#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < len; ++i) {
        sum += A[i];
    }
    return sum;
}

#endif
