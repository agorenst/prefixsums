#include "prefix_sums.h"
#include <iostream>

using namespace std;

int main() {
    int a[8] = {1,2,3,4,5,6,7,8};
    prefix_sums(a,8);
    print_array(a,8);
}
