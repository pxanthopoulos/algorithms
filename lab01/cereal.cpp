#include <iostream>

using namespace std;

//checks if a number is indeed an existing median value
bool median_exists (int a[], int N, int K, int median) {
    //array that indicates if the corresponding value at array a is higher or
    //lower than the number
    int low_high[N];
    for (int i = 0; i < N; i++) {
        if (a[i] >= median) {
            low_high[i] = +1;
        }
        else {
            low_high[i] = -1;
        }
    }
    //subset sum, low_high array now contains number of lower and upper values so far
    for (int i = 1; i < N; i++) {
        low_high[i] += low_high[i-1];
    }

    //if a contiguous subarray of size K has more values greater or equal than the 
    //number, then the number is an existing median value
    int pos = K;
    int start = 0;
    int end = low_high[K - 1];
    while (pos < N) {
        start = min(low_high[pos - K], start);
        end = max(end, low_high[pos] - start);
        pos ++;
    }
    if (end > 0) {
        return true;
    }
    else {
        return false;
    }
}

int main() {
    //read input
    int N;
    cin >> N;
    int K;
    cin >> K;
    int a[N];
    for (int i = 0; i < N; i++) {
        cin >> a[i];
    }

    //binary search through possible values of max median (1 to N)
    //if a value is valid, update the current max median and search for higher values
    //if not, search for lower values
    int left = 1;
    int right = N+1;
    int best = -1;
    
    while (left <= right) {
        int mid = (left + right )/2;
        if (median_exists(a, N, K, mid)) {
            best = mid;
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    cout << best << endl;
}