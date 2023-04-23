#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

// bfs algorithm for a given adjacency list and a given starting node
vector<int> bfs(vector<vector<pair<int, int>>> &adj, int start_node)
{
    vector<bool> visited(adj.size(), false);
    vector<int> distances(adj.size(), -1);
    queue<int> q;

    q.push(start_node);
    visited[start_node] = true;
    distances[start_node] = 0;

    while (!q.empty())
    {
        int curr_node = q.front();
        q.pop();

        for (auto neighbor : adj[curr_node])
        {
            int neighbor_ = neighbor.first;
            if (!visited[neighbor_])
            {
                q.push(neighbor_);
                visited[neighbor_] = true;
                distances[neighbor_] = distances[curr_node] + 1;
            }
        }
    }
    return distances;
}
// dijkstra's algorithm for a given adjacency list and a given starting node
vector<int> dijkstra(vector<vector<pair<int, int>>> &adj, int start_node)
{
    vector<int> distances(adj.size(), numeric_limits<int>::max());
    vector<int> visited(adj.size(), 0);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    distances[start_node] = 0;
    pq.push(make_pair(0, start_node));

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        if (visited[u])
        {
            continue;
        }

        for (pair<int, int> neighbor : adj[u])
        {
            long int v = neighbor.first;
            long int w = neighbor.second;
            if (distances[v] > distances[u] + w)
            {
                distances[v] = distances[u] + w;
                pq.push({distances[v], v});
            }
        }
    }

    return distances;
}

// we use 1 indexed vertices for convenience (since the input vertices start also from 1)
int main(void)
{
    int N, M, s, t, B;
    cin >> N >> M >> s >> t >> B;

    vector<vector<pair<int, int>>> adj(N + 1);

    for (int i = 0; i < M; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    vector<int> dist = bfs(adj, s);

    // the maximum number of skips that we could need is the number of edges between s and t
    // this number is provided by the bfs algorithm
    int max_skip = dist[t];

    // we then create a supergraph with multiple levels
    // each level is a copy of the original graph
    // for each edge (u,v), we also connect u to the corresponding v of the above level, with cost 0
    // this edge represents a skip
    // max number of skips is the levels of the supergraph minus 1
    int levels = max_skip + 1;

    vector<vector<pair<int, int>>> new_adj(levels * N + 1);

    for (size_t i = 0; i < adj.size(); i++)
    {
        for (size_t j = 0; j < adj[i].size(); j++)
        {
            pair<int, int> p = adj[i][j];
            for (int k = 0; k < levels; k++)
            {
                pair<int, int> p2 = p;
                p2.first += k * N;
                new_adj[k * N + i].push_back(p2);

                if (k != levels - 1)
                {
                    p2.first += N;
                    p2.second = 0;
                    new_adj[k * N + i].push_back(p2);
                }
            }
        }
    }

    // we call dijkstra's algo for the supergraph
    vector<int> distances = dijkstra(new_adj, s);

    int skip = 0;

    // the distance of the t vertex at the bottom (0) level corresponds to the shortest path from s to t if we skip 0 edges
    // the distance of the t vertex at level 1 corresponds to the shortest path from s to t if we skip 1 edges
    // etc
    // so we check the t vertex of each level until we find one whose distance is <= B
    // the level that it belongs to is the number of skips needed
    for (int i = 0; i < levels; i++)
    {
        if (distances[t + i * N] <= B)
        {
            skip = i;
            break;
        }
    }

    cout << skip << endl;

    return 0;
}