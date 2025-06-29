#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <string>

using namespace std;

const int INF = 1e9;

struct Edge {
    int to;
    int time;
    int cost;
};

class Graph {
private:
    unordered_map<int, vector<Edge>> adj;

public:
    void addEdge(int from, int to, int time, int cost) {
        adj[from].push_back({to, time, cost});
    }

    const vector<Edge>& getNeighbors(int node) const {
        static vector<Edge> empty;
        auto it = adj.find(node);
        return it != adj.end() ? it->second : empty;
    }

    vector<int> dijkstra(int start, int n, bool useTime = true) const {
        vector<int> dist(n, INF);
        dist[start] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [curDist, u] = pq.top(); pq.pop();
            if (curDist > dist[u]) continue;

            for (const Edge& e : getNeighbors(u)) {
                int weight = useTime ? e.time : e.cost;
                if (dist[u] + weight < dist[e.to]) {
                    dist[e.to] = dist[u] + weight;
                    pq.push({dist[e.to], e.to});
                }
            }
        }
        return dist;
    }

    void printGraph() const {
        for (const auto& [u, edges] : adj) {
            for (const auto& e : edges) {
                cout << "From " << u << " -> " << e.to
                     << " (Time: " << e.time << ", Cost: " << e.cost << ")\n";
            }
        }
    }
};

#endif
