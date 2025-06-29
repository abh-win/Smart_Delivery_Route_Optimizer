#ifndef PLANNER_H
#define PLANNER_H

#include "graph.h"
#include "delivery.h"

#include <vector>
#include <unordered_map>
#include <bitset>
#include <limits>
#include <algorithm>

using namespace std;

struct RouteResult {
    vector<int> deliveryOrder;
    int totalCost;
    int totalTime;
    bool feasible;
};

class DeliveryPlanner {
private:
    const Graph& graph;
    const DeliveryManager& dm;
    int cityCount;
    unordered_map<int, unordered_map<int, vector<int>>> pathCache;

public:
    DeliveryPlanner(const Graph& g, const DeliveryManager& d, int n) : graph(g), dm(d), cityCount(n) {}

    RouteResult planRoute(int startCity) {
        vector<int> ids;
        for (auto& [id, _] : dm.getAllDeliveries()) ids.push_back(id);
        int m = ids.size();

        vector<vector<int>> dist(cityCount, vector<int>(cityCount, INF));
        for (int u = 0; u < cityCount; ++u) {
            vector<int> dists = graph.dijkstra(u, cityCount, true);
            for (int v = 0; v < cityCount; ++v)
                dist[u][v] = dists[v];
        }

        vector<vector<int>> cost(cityCount, vector<int>(cityCount, INF));
        for (int u = 0; u < cityCount; ++u) {
            vector<int> costs = graph.dijkstra(u, cityCount, false);
            for (int v = 0; v < cityCount; ++v)
                cost[u][v] = costs[v];
        }

        int full = 1 << m;
        vector<vector<int>> dp(full, vector<int>(cityCount, INF));
        vector<vector<int>> costDP(full, vector<int>(cityCount, INF));
        vector<vector<int>> parent(full, vector<int>(cityCount, -1));
        vector<vector<int>> lastDelivered(full, vector<int>(cityCount, -1));

        dp[0][startCity] = 0;
        costDP[0][startCity] = 0;

        for (int mask = 0; mask < full; ++mask) {
            for (int u = 0; u < cityCount; ++u) {
                if (dp[mask][u] == INF) continue;

                for (int i = 0; i < m; ++i) {
                    if (mask & (1 << i)) continue;

                    int id = ids[i];
                    const Delivery* d = dm.getDelivery(id);
                    if (!d) continue;

                    bool depsOk = true;
                    for (int dep : d->dependencies) {
                        auto it = find(ids.begin(), ids.end(), dep);
                        if (it == ids.end()) continue;
                        int j = it - ids.begin();
                        if (!(mask & (1 << j))) {
                            depsOk = false;
                            break;
                        }
                    }
                    if (!depsOk) continue;

                    int to = d->destination;
                    int travelTime = dist[u][d->source] + dist[d->source][to];
                    int travelCost = cost[u][d->source] + cost[d->source][to];
                    int newTime = dp[mask][u] + travelTime;
                    int newCost = costDP[mask][u] + travelCost;

                    if (newTime > d->deadline || newCost > d->budget) continue;

                    int newMask = mask | (1 << i);
                    if (newTime < dp[newMask][to]) {
                        dp[newMask][to] = newTime;
                        costDP[newMask][to] = newCost;
                        parent[newMask][to] = u;
                        lastDelivered[newMask][to] = id;
                    }
                }
            }
        }

        int minTime = INF, endCity = -1, bestMask = full - 1;
        for (int u = 0; u < cityCount; ++u) {
            if (dp[bestMask][u] < minTime) {
                minTime = dp[bestMask][u];
                endCity = u;
            }
        }

        if (endCity == -1) {
            return {{}, -1, -1, false};
        }

        vector<int> route;
        int mask = bestMask, city = endCity;
        while (mask > 0) {
            int id = lastDelivered[mask][city];
            if (id != -1) route.push_back(id);
            int prev = parent[mask][city];
            for (int i = 0; i < m; ++i) {
                if ((mask & (1 << i)) && ids[i] == id) {
                    mask ^= (1 << i);
                    break;
                }
            }
            city = prev;
        }

        reverse(route.begin(), route.end());
        return {route, costDP[bestMask][endCity], dp[bestMask][endCity], true};
    }
};

#endif
