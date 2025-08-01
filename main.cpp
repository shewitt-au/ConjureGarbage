#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

template <typename T>
void print_vector(const vector<T> &vec) {
    for (auto i=0; i<vec.size(); ++i) {
        if (i!=0 && i%10 == 0)
            cout << endl;
        cout << setw(3) << vec[i] << " ";
    }
    cout << endl;
}

int main()
{
    vector<int> reference_vec;
    vector<int> shuffled;

    for (int i=1; i<=100; ++i) {
        reference_vec.push_back(i);
        shuffled.push_back(i);
    }

    cout << "Reference vector:" << endl;
    print_vector(reference_vec);

    random_device rd;
    mt19937 g(rd());

    size_t times_shuffled = 1;
    vector<int> pre_sort;
    for (; times_shuffled<=1000; ++times_shuffled) {
        shuffle(shuffled.begin(), shuffled.end(), g);

        pre_sort = shuffled;
        sort(shuffled.begin(), shuffled.end(), [](int l, int r) {
            if (l%5==0 && r%5==0)
                return true; // violate strict weak ordering if both divisible by 5

            return l<r;
        });

        if (!is_permutation(reference_vec.begin(), reference_vec.end(), shuffled.begin())) {
            ++times_shuffled;
            break;
        }
    }

    cout << endl << "Times shuffled: " << times_shuffled-1 << endl << endl;
    cout << "vector before sorting:" << endl;
    print_vector(pre_sort);
    cout << endl;
    cout << "vector after sorting:" << endl;
    print_vector(shuffled);
    cout << endl;

    return 0;
}
