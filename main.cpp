#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

int main()
{
    using namespace std;

    vector<int> sorted;
    vector<int> shuffled;

    for (int i=1; i<=100; ++i) {
        sorted.push_back(i);
        shuffled.push_back(i);
    }

    random_device rd;
    mt19937 g(rd());

    for (;;) {
        shuffle(shuffled.begin(), shuffled.end(), g);

        sort(shuffled.begin(), shuffled.end(), [](int l, int r) {
            if (l%5==0 && r%5==0)
                return true;

            return l<r;
        });

        if (!is_permutation(sorted.begin(), sorted.end(), shuffled.begin())) {
            for (auto i=0; i<shuffled.size(); ++i) {
                if (i!=0) cout << ", ";
                std::cout << shuffled[i];
            }
            
            return 0;
        }
    }

    return 0;
}
