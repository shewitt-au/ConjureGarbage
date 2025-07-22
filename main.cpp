#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    using namespace std;

    vector<int> a{1, 2, 3};
    vector<int> b{3, 2, 1};

    if (is_permutation(a.begin(),a.end(), b.begin()))
        cout << "==" << endl;
    else
        cout << "!=" << endl;

    return 0;
}