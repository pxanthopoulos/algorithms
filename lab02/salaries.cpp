/*  The problem requires us to find the optimal interval [l,r] and the optimal
 *  x, where -K <= x <= K, that create the longest possible LIS. We observe 
 *  that if x is always K and if the interval is always [l, N-1] (optimize
 *  only the beginning), the longest possible LIS remains of the same length.
 */

/*  Our algorithm first computes the LIS lengths, using all N numbers.
 *  Then, for i=N-1, N-2, ..., 1, we remove the element arr[i] from the 
 *  LIS array, and we add it to the LDS array. For this "undo" process, we
 *  use a history vector for the LIS array, where we hold the previous values
 *  and the index of arr to which they correspond to. Every time, we compute
 *  the best length by finding the first element of the LIS array that is lower 
 *  than the last element of the LDS array.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

//  arr contains the values of the salaries
int arr[200000];

/*  This array contains the values of the smallest endings of LIS at given lengths 
 *  If, for example, 6 is at index 2, then, out of all the increasing subsequences
 *  of length 3 (index 2 +1), 6 is the least of all of their last elements. The 
 *  second column holds the index of arr to which the value corresponds to. If we want to
 *  remove (undo) the value arr[i], which exists at the LIS array at index 7, we only remove it
 *  if LIS[7][1] == i, that is if it came from index i.
 */
int LIS_lengths_array[200000][2];
int LIS_lengths_array_index = 0;

/*  The same but for decreasing subsequence, but the largest of all endings /
 *  last elements.
 */
int LDS_lengths_array[200000];
int LDS_lengths_array_index = 0;

/*  Undo history for LIS. If we want to remove a value from the LIS array, we need
 *  to keep the previous values at that position. Also, we keep the index of arr that
 *  correspond to that value.
 */
vector<vector<tuple<int,int>>> LIS_history(200000);

//  add new element to the LIS lengths array
void BSreplace_high(int low, int high, int x, int index)
{
    if (high >= low)
    {
        int mid = low + ((high - low) / 2);

        if (LIS_lengths_array[mid][0] == x)
        {
            return;
        }
        else if (x < LIS_lengths_array[mid][0])
        {
            BSreplace_high(low, mid - 1, x, index);
        }
        else
        {
            BSreplace_high(mid + 1, high, x, index);
        }
    }
    else
    {
        if (low == LIS_lengths_array_index)
        {
            LIS_lengths_array[low][0] = x;
            LIS_lengths_array[low][1] = index;
            LIS_lengths_array_index++;
        } else
        {   
            tuple<int,int> temp = make_tuple(LIS_lengths_array[low][0], LIS_lengths_array[low][1]);
            LIS_history[low].push_back(temp);
            LIS_lengths_array[low][0] = x;
            LIS_lengths_array[low][1] = index;
        }
        return;
    }
}

//  Do this for all elements of arr
void LIS_lengths(int start, int end)
{
    for (int i = start; i <= end; i++)
    {
        BSreplace_high(0, LIS_lengths_array_index - 1, arr[i], i);
    }
    return;
}

//  add new element to the LDS lengths array
void BSreplace_low(int low, int high, int x)
{
    if (high >= low)
    {
        int mid = low + ((high - low) / 2);

        if (LDS_lengths_array[mid] == x)
        {
            return;
        }
        else if (x > LDS_lengths_array[mid])
        {
            BSreplace_low(low, mid - 1, x);
        }
        else
        {
            BSreplace_low(mid + 1, high, x);
        }
    }
    else
    {
        if (low == LDS_lengths_array_index)
        {
            LDS_lengths_array[low] = x;
            LDS_lengths_array_index++;
        } else
        {
            LDS_lengths_array[low] = x;
        }
        return;
    }
}

//find and remove (undo) the given value from the LIS length array
void BSundo(int x, int index) {
    int left = 0;
    int right = LIS_lengths_array_index - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (LIS_lengths_array[mid][0] == x)
        {
            if (LIS_lengths_array[mid][1] != index)
            {
                return;
            }
            if (mid == LIS_lengths_array_index - 1)
            {
                if (!LIS_history[mid].empty())
                {   
                    LIS_lengths_array[mid][0] = get<0>(LIS_history[mid].back());
                    LIS_lengths_array[mid][1] = get<1>(LIS_history[mid].back());
                    LIS_history[mid].pop_back();
                } else 
                {
                    LIS_lengths_array_index--;
                }
            }
            else
            {
                LIS_lengths_array[mid][0] = get<0>(LIS_history[mid].back());
                LIS_lengths_array[mid][1] = get<1>(LIS_history[mid].back());
                LIS_history[mid].pop_back();
            }
            return;
        } else if (LIS_lengths_array[mid][0] < x)
        {
            left = mid + 1;
        } else
        {
            right = mid - 1;
        }
    }
    return;
}

int findLargestSmallerThanX(int x) {
    int left = 0;
    int right = LIS_lengths_array_index - 1;
    int largestSmaller = -1; // to store the position of the largest value that is smaller than x

    while (left <= right) {
            int mid = left + (right - left) / 2;
            if (LIS_lengths_array[mid][0] < x)
            {
                largestSmaller = mid;
                left = mid + 1;
            } else
            {
                right = mid - 1;
            }
    }
    return largestSmaller;
}

int main(void)
{
    int N, K;

    cin >> N >> K;

    for (int i = 0; i < N; i++)
    {
        cin >> arr[i]; 
    }

    for (int i = 0; i < N; i++)
    {
        LIS_lengths_array[i][1] = -1;
    }

    LIS_lengths(0, N-1);

    int curr_length = LIS_lengths_array_index;

    for (int i = N-1; i >= 0; i--)
    {   
        BSreplace_low(0, LDS_lengths_array_index - 1, arr[i] + K);
        BSundo(arr[i], i);

        int pos =  findLargestSmallerThanX(LDS_lengths_array[LDS_lengths_array_index-1]);

        if (curr_length < LDS_lengths_array_index + pos + 1)
        {
            curr_length = LDS_lengths_array_index + pos + 1;
        }
    }
    cout << curr_length << endl;
}