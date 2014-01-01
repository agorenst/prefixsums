#include "prefix_sums.h"
#include <iostream>

using namespace std;

template<typename T>
bool arrays_equal(T* a, T* b, int n) {
    bool res = true;
    for (auto i = 0; i < n; ++i) {
        res = res && (a[i] == b[i]);
    }
    return res;
}

int main(int argc, char* argv[]) {
    auto n = atoi(argv[1]);


    long long* a = new long long[n];
    long long* b = new long long[n];
    for (auto i = 0; i < n; ++i) {
        a[i] = rand() % 100;
        b[i] = a[i];
    }

//    clock_t start1,start2;
//    double diff1,diff2;
//    start1 = clock();
//    prefix_sums(a,n);//,mem,store);
//    diff1 = (clock() - start1);
//    start2 = clock();

    serial_sums(b,n);

//    diff2 = (clock() - start2);
//    cout << arrays_equal(a,b,n) << endl;
//    cout << diff1 << "\t" << diff2 << endl;
    delete [] a;
    delete [] b;
}
