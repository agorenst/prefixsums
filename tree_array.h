/*
 * In most implementations of prefix sums, we essentially create
 * a binary tree.
 * This struture is thta.
 */


#include <vector>
#include <cmath>

using namespace std;

template<typename T>
class tree_array {
    private:
        T* leaves; // this is the array provided, ult. modified to have sum
        T* nodes;
        size_t* height_lengths; // how many nodes on level x?
        size_t* height_offsets; // this is used in indexing

        size_t compute_ncount() {
            height_lengths = new size_t[height];
            height_offsets = new size_t[height];
            height_lengths[0] = lcount;
            size_t tmp_count = 0;
            for (size_t i = 1; i < height; ++i) {
                // see http://stackoverflow.com/questions/2745074/fast-ceiling-of-an-integer-division-in-c-c
                auto ceil = (height_lengths[i-1]+1)/2;
                height_lengths[i] = ceil;
                tmp_count += ceil;
            }

            height_offsets[0] = 0;
            for (size_t i = 1; i < height; ++i) {
                height_offsets[i] = height_offsets[i-1]+height_lengths[i-1];
            }

            return tmp_count;
        }

        size_t compute_height() const {
            int k = 0;
            while ((1 << k) <= lcount) { ++k; } // find essentially log(n);
            return k;
        }

    public:
        const size_t lcount; // how many leaves (original input)?
        const size_t height; // how many levels of the tree
        const size_t ncount; // how many inner nodes are there

        // initialize the basic values.
        tree_array(T* leaves, int n): leaves(leaves),
        lcount(n), height(compute_height()), ncount(compute_ncount()) {
            nodes = new T[ncount];
        }
};

/*
template<typename T>
class cont_tree_array {
    private:
        T* nodes;
        T* leaves;

        int compute_height() const {
            int k = 0;
            while ((1 << k) <= lcount) { ++k; } // find essentially log(n);
            return k;
        }

        int compute_nodecount() const {
            return pow(2,height-1)-1;
        }

    public:
        int lcount;
        int height;
        int ncount;
        cont_tree_array(T* leaves, int n):
            leaves(leaves), lcount(n), height(compute_height()), ncount(compute_nodecount()) {
                nodes = new T[ncount];
            }
        T& operator()(int level, int index) {
            if (level == 0) {
                return leaves[index];
            }
            else {
                int ind = pow(2,(height-(1+level)))-1+index;
                return nodes[ind];
            }
        }
        ~cont_tree_array() {
            delete nodes;
        }
};

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
*/
