#include <string>
#include "par_fns.h"
#include "array_utils.h"

using namespace std;

bool test_prefix_sums(unsigned n = 1000) {
    int* test = new int[n];
    int* test_res = new int[n];
    rand_array(test, n);
    int* control = new int[n];
    int* control_res = new int[n];
    copy_array(test, control, n);
    
    prefix_sums(test, test_res, n);
    
    control_res[0] = control[0];
    for (unsigned i = 1; i < n; ++i) {
        control_res[i] = control[i]+control_res[i-1];
    }
    return equal_arrays(test_res, control_res, n);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    cout << test_prefix_sums(atoi(argv[1])) << endl;
    return 0;
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
