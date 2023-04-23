#include <iostream>
#include <vector>
#include <map>
#include <utility>

using namespace std;

// union find class that uses union by rank and path compression
// also holds the size of each connected component and the maximum size of the components
class UnionFind
{
public:
    // Constructor that initializes the data structure
    UnionFind(int size)
    {
        parent_.resize(size);
        rank_.resize(size, 0);
        size_.resize(size, 1);
        max_size_ = 1;
        count_ = size;
        for (int i = 0; i < size; i++)
        {
            parent_[i] = i;
        }
    }

    // Find the root of the element with path compression
    int find(int x)
    {
        if (parent_[x] != x)
        {
            parent_[x] = find(parent_[x]);
        }
        return parent_[x];
    }

    // Union two elements by rank
    void unite(int x, int y)
    {
        int root_x = find(x);
        int root_y = find(y);
        if (root_x != root_y)
        {
            if (rank_[root_x] < rank_[root_y])
            {
                parent_[root_x] = root_y;
                size_[root_y] += size_[root_x];
                max_size_ = max(max_size_, size_[root_y]);
            }
            else if (rank_[root_x] > rank_[root_y])
            {
                parent_[root_y] = root_x;
                size_[root_x] += size_[root_y];
                max_size_ = max(max_size_, size_[root_x]);
            }
            else
            {
                parent_[root_y] = root_x;
                rank_[root_x]++;
                size_[root_x] += size_[root_y];
                max_size_ = max(max_size_, size_[root_x]);
            }
            count_--;
        }
    }

    // Get the number of connected components
    int count()
    {
        return count_;
    }

    // Get the size of the component containing element x
    int size(int x)
    {
        int root_x = find(x);
        return size_[root_x];
    }

    // Get the maximum size of the connected components
    int max_size()
    {
        return max_size_;
    }

private:
    vector<int> parent_;
    vector<int> rank_;
    vector<int> size_;
    int max_size_;
    int count_;
};

vector<int> P(63); // P values, inital values are 63
int idx = 63;      // current idx
int N, F;

// function that generates a new P value, based on the current index
void generateP(void)
{
    idx++;
    int Pval = (P[idx - 1 - 63] + P[idx - 1 - 31]) % N;
    P.push_back(Pval);
    return;
}

int main(void)
{
    cin >> N >> F;

    float perc = ((float)F / 100) * float(N);

    for (int i = 0; i < idx; i++)
    {
        int num;
        cin >> num;
        P[i] = num;
    }
    generateP(); // add one value to P so that it has an even size

    // initalize union find class with initally N connected components
    UnionFind uf(N);

    int loopcount = 0;

    // a map that maps an unordered pair to a frequency
    // we insert and search each pair of P values using the pair
    // max,min so that the pairs are unordered
    map<pair<int, int>, int> freq;
    map<pair<int, int>, int>::iterator it;
    int dupcount = 0;

    int count = 0;
    int finished = 0; /* boolean to indicate if we need  *
                       * to generate more pairs after we *
                       * checked the first 64 P values   */

    for (int i = 1; i <= idx / 2; i++)
    {
        // check for loops
        if (P[i * 2 - 2] == P[i * 2 - 1])
        {
            loopcount++;
        }

        // check for duplicates
        pair<int, int> p = make_pair(max(P[i * 2 - 2], P[i * 2 - 1]), min(P[i * 2 - 2], P[i * 2 - 1]));
        it = freq.find(p);
        if (it == freq.end())
        {
            // if pair was not found, insert it with frequency 1
            freq.insert(pair<pair<int, int>, int>(p, 1));
        }
        else
        {
            // else, if it was found once more, increase duplicate count
            if (it->second == 1)
            {
                dupcount++;
            }
            it->second++;
        }

        // update union find structure
        uf.unite(P[i * 2 - 2], P[i * 2 - 1]);
        if (uf.max_size() >= perc)
        {
            count = i;
            finished = 1;
            break;
        }
    }

    if (finished == 0)
    {
        count = idx / 2;
        while (uf.max_size() < perc)
        {
            // generate a new pair
            generateP();
            generateP();
            count++;

            // check for loops
            if (P[2 * count - 2] == P[2 * count - 1])
            {
                loopcount++;
            }

            // check for duplicates
            pair<int, int> p = make_pair(max(P[2 * count - 2], P[2 * count - 1]), min(P[2 * count - 2], P[2 * count - 1]));
            it = freq.find(p);
            if (it == freq.end())
            {
                // if pair was not found, insert it with frequency 1
                freq.insert(pair<pair<int, int>, int>(p, 1));
            }
            else
            {
                // else, if it was found once more, increase duplicate count
                if (it->second == 1)
                {
                    dupcount++;
                }
                it->second++;
            }

            // update union find structure
            uf.unite(P[2 * count - 2], P[2 * count - 1]);
        }
    }

    // final number of pairs needed is count
    // minus the loops and number of double pairs

    cout << count - dupcount - loopcount << " " << dupcount << " " << loopcount << endl;

    return 0;
}