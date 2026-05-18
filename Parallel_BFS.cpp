#include <iostream>
#include <vector>
#include <queue>
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

    void addEdge(int v, int w) {
        adj[v].push_back(w);
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int size = q.size();

            #pragma omp parallel for
            for (int i = 0; i < size; i++) {
                int v;

                #pragma omp critical
                {
                    if (!q.empty()) {
                        v = q.front();
                        q.pop();
                        cout << v << " ";
                    }
                }

                for (int n : adj[v]) {
                    if (!visited[n]) {
                        #pragma omp critical
                        {
                            if (!visited[n]) {
                                visited[n] = true;
                                q.push(n);
                            }
                        }
                    }
                }
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

    cout << "BFS: ";
    g.parallelBFS(start);
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
BFS: 0 1 2 3 4 5 6
*/
