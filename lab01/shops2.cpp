#include <iostream>

using namespace std;

int main() {
    //read input
    int N;
    cin >> N;
    int K;
    cin >> K;
    int population[N];
    for (int i = 0; i < N; i++) {
        cin >> population[i];
    }

    //sums[i] holds the minimum length of contiguous houses add up to polulation sums[i]

    int sums[K+1];

    // initalize sums to length higher than the worst possible
    // only sums[0] is initalized to 0

    for (int i = 0; i < K+1; i++) {
        sums[i] = i == 0 ? 0 : N+1;
    }

    // result, initalized at length higher than the worst possible, if N+1 at the end, print -1

    int res = N+1;

    // i is the axis and we check around i for sums

    for (int i = 0; i < N; i++) {

        int curr_sum = 0, curr_len = 0;

        // check for contiguous subarrays to the left of i 
        // we traverse the array backwards so that we dont check the same elements every time

        for (int j = i-1; j >=0; j--) {

            curr_sum += population[j];
            
            // if current sum exceeds or is K, break because we want at least one element 
            // that is to the right of i

            if (curr_sum >= K) {
                break;
            }

            curr_len++;
            
            // if current legth is lower than the lowest currently saved at sums at position current sum
            // then update

            sums[curr_sum] = curr_len < sums[curr_sum] ? curr_len : sums[curr_sum];
        }

        curr_sum = curr_len = 0;

        // now we check for contiguous subarrays that start at i and go to the right

        for (int k = i; k < N; k++) {

            curr_sum += population[k];

            // if current sum exceeds K then we dismiss current i 
            // if current sum is K, then the other contiguous subarray will have length zero
            // which means we basically selected only 1 coniguous subarray and not 2

            if (curr_sum > K) {
                break;
            }
            
            curr_len++;
            
            // if current length + the minimum length of sum: K - current sum is lower 
            // than our current result, then update result

            res = (sums[K-curr_sum] + curr_len < res ? sums[K-curr_sum] + curr_len : res);
        }

    }

    // if res stayed at value N+1, that means we never found 1 or 2 contiguous subarrays
    // that sum exactly to K, so we print -1

    cout << (res == N+1 ? -1 : res) << endl;
}