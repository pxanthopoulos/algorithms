#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

const uint64_t MAX_SIZE = 100000;
uint64_t distances[MAX_SIZE];
queue<uint64_t> q;

void find_distances(vector<vector<pair<uint64_t,uint64_t>>> &adj, vector<vector<uint64_t>> &paths)
{
    distances[0] = 0;
    
    q.push(1);
    
    while (!q.empty()) {
        uint64_t u = q.front();
        q.pop();
    
        for (auto edge : adj[u]) {
            uint64_t v = edge.first;
            uint64_t w = edge.second;
    
            if (distances[v-1] > distances[u-1] + w) {
                distances[v-1] = distances[u-1] + w;
                q.push(v);
                paths[v] = paths[u];
                paths[v].push_back(v);
            }
        }
    }
    return;
}

void update_adj(vector<vector<pair<uint64_t,uint64_t>>> &adj, vector<pair<uint64_t, uint64_t>> &helpers, uint64_t N, vector<vector<uint64_t>> &paths)
{
    for (uint64_t i = 1; i <= N; i++)
    {
        adj[i].clear();
    }
    for (uint64_t i = 2; i <= N; i++)
    {
        for (size_t j = 0; j < paths[i].size() - 1; j++)
        {   
            uint64_t v = paths[i][j];
            if (v < i)
            {
                uint64_t cost = (uint64_t)(helpers[i].first + helpers[i].second * (distances[i-1] - distances[v-1]));
                adj[v].push_back({i, cost});
            }  
        }
    }
    return;
}

vector<vector<uint64_t>> paths(MAX_SIZE+1);
vector<vector<pair<uint64_t,uint64_t>>> adj(MAX_SIZE+1);
vector<pair<uint64_t, uint64_t>> helpers(MAX_SIZE+1);

int main(void) {
    uint64_t N;
    cin >> N;

    paths[1].push_back(1);

    for (uint64_t i = 0; i < N-1; i++) {
        uint64_t u, v, w;
        cin >> u >> v >> w;
        if (u > v)
        {
            adj[v].push_back({u, w});
        } else 
        {
            adj[u].push_back({v, w});
        }
    }

    for (uint64_t i = 0; i < N-1; i++) {
        uint64_t p, s;
        cin >> p >> s;
        helpers[i+2] = {p, s};
    }

    for (uint64_t i = 0; i < N; i++)
    {
        distances[i] = UINT64_MAX;
    }

    find_distances(adj, paths);

    update_adj(adj, helpers, N, paths);    

    for (uint64_t i = 0; i < N; i++)
    {
        distances[i] = UINT64_MAX;
    }

    find_distances(adj, paths);

    cout << distances[1];

    for (uint64_t i = 2; i < N; i++)
    {
        cout << " " << distances[i];
    }
    cout << endl;

    return 0;
}