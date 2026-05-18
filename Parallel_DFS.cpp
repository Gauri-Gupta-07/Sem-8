#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    void dfsUtil(int v, vector<bool>& visited) {
        bool alreadyVisited = false;

        // Critical section to safely check & mark visited
        #pragma omp critical
        {
            if (visited[v]) {
                alreadyVisited = true;
            } else {
                visited[v] = true;
                cout << v << " ";
            }
        }

        if (alreadyVisited) return;

        // Create tasks for neighbors
        for (int n : adj[v]) {
            #pragma omp task
            dfsUtil(n, visited);
        }
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);

        #pragma omp parallel
        {
            #pragma omp single
            {
                dfsUtil(start, visited);
            }
        }
    }
};

int main() {
    int V, E;
    cout << "Enter vertices and edges: ";
    cin >> V >> E;

    Graph g(V);

    cout << "Enter edges (u v):\n";
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start;
    cout << "Enter starting node: ";
    cin >> start;

    cout << "DFS: ";
    g.parallelDFS(start);

    return 0;
}



/*
OUTPUT :

Enter vertices and edges: 7 6
Enter edges (u v):
0 1
0 2
1 3
1 4
2 5
2 6
Enter starting node: 0
DFS: 0 1 3 4 2 5 6
*/
