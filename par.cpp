#include <string>
//#include "par_fns.h"
#include "array_utils.h"
#include "prefix_sums.h"

using namespace std;

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

void time_prefix_sums(unsigned n = 100000000) {
    int* test = new int[n];
    int* test_res = new int[n];
    rand_array(test, n);
    prefix_sums<int*, int*, int>(test, test+n, test_res);
}

bool test_cont_sum(unsigned n = 100000000) {
    int* test = new int[n];
    rand_array(test, n);
    int* control = new int[n];
    int* control_res = new int[n];
    copy_array(test, control, n);

    cont_prefix_sums(test, n);

    control_res[0] = control[0];
    for (unsigned i = 1; i < n; ++i) {
        control_res[i] = control[i]+control_res[i-1];
    }

    //print_array(test_res, n);
    //print_array(control_res, n);

    auto res = equal_arrays(test, control_res, n);
    delete test;
    delete control;
    delete control_res;
    return res;
}

bool test_prefix_sums(unsigned n = 100000000) {
    int* test = new int[n];
    int* test_res = new int[n];
    rand_array(test, n);
    int* control = new int[n];
    int* control_res = new int[n];
    copy_array(test, control, n);

    //print_array(test, n);
    //print_array(control, n);

    smaller_prefix_sums<int*, int*, int>(test, test+n, test_res);

    control_res[0] = control[0];
    for (unsigned i = 1; i < n; ++i) {
        control_res[i] = control[i]+control_res[i-1];
    }

    //print_array(test_res, n);
    //print_array(control_res, n);

    return equal_arrays(test_res, control_res, n);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    //time_prefix_sums();
    cout << test_cont_sum(atoi(argv[1])) << endl;
    return 0;
} /*
     const int len = 10;
     int test[len];
//= {"a", "b", "c", "d", "e", "f", "g", "h","i", "k", "l", "m", "n", "o", "p", "q"};
int scratch[len];
for (int i = 0; i < 100; ++i) {
rand_array(test, len);
sync_array<int> stest(test, len);
in_place_prefix_sums(test, scratch, len);
new_in_place(stest);
for (int i = 0; i < stest.n; ++i) {
cout << scratch[i] << " " << stest[i] << std::endl;
}
}
}
   */
