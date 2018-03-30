#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Illegal Paramenter" << endl;
        return -1;
    }
    int n = atoi(argv[1]);
    // remember to remove 0 and 1
    vector<bool> prime = vector<bool>(n + 1, true);
    int loopEnd = (int)floor(sqrt(n));
    for (int num = 2; num <= loopEnd; ++num) {
        if (prime[num]) {
            for (int loc = num << 1; loc <= n; loc += num) {
                prime[loc] = false;
            }
        }
    }
    // count
    int count = 0;
    for (int i = 2; i <= n; ++i) {
        if (prime[i]) {
            ++count;
        }
    }
    cout << "There are " << count << " primes before " << n << endl;
    return 0;
}