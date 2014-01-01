/*
 * In most implementations of prefix sums, we essentially create
 * a binary tree.
 * This is that structure.
 *
 *
 * NOTE: There is a nontriviality, in the sense that we always assume
 * the distance between begin and end is a power of two.
 */


#include <vector>
#include <cmath>

// 
template<typename T>
class tree_array {
    private:
        T** store;
    public:
        int n;
        int height;
        template<typename iter>
            tree_array(iter begin, iter end) {
                vector<T> data(begin, end);
                n = data.size();
                unsigned k = 0;
                while ((1 << k) < n) { ++k; } // find essentially log(n);
                height = k;
                store = new T*[k+1]; // the height of our tree

                // initialize each level of the tree
                // TODO: instead, make store operate on one contiguious table.
                for (unsigned h = 0; h <= k; ++h) {
                    store[h] = new T[(1 << (k-h))];
                }

                copy(data.begin(), data.end(), store[0]);
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
