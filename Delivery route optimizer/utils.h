#ifndef UTILS_H
#define UTILS_H

#include "delivery.h"

#include <unordered_map>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

vector<int> topologicalSort(const DeliveryManager& dm) {
    unordered_map<int, int> inDegree;
    unordered_map<int, vector<int>> graph;

    for (const auto& [id, delivery] : dm.getAllDeliveries()) {
        if (!inDegree.count(id)) inDegree[id] = 0;
        for (int dep : delivery.dependencies) {
            graph[dep].push_back(id);
            inDegree[id]++;
        }
    }

    queue<int> q;
    for (const auto& [id, deg] : inDegree) {
        if (deg == 0) q.push(id);
    }

    vector<int> order;
    while (!q.empty()) {
        int curr = q.front(); q.pop();
        order.push_back(curr);

        for (int neighbor : graph[curr]) {
            if (--inDegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    if (order.size() != dm.getAllDeliveries().size()) return {}; // Cycle detected
    return order;
}

#endif
