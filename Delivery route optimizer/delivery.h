#ifndef DELIVERY_H
#define DELIVERY_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Delivery {
    int id;
    int source;
    int destination;
    int deadline;
    int budget;
    vector<int> dependencies;
};

class DeliveryManager {
private:
    unordered_map<int, Delivery> deliveries;

public:
    void addDelivery(int id, int source, int destination, int deadline, int budget, const vector<int>& deps = {}) {
        deliveries[id] = {id, source, destination, deadline, budget, deps};
    }

    const unordered_map<int, Delivery>& getAllDeliveries() const {
        return deliveries;
    }

    const Delivery* getDelivery(int id) const {
        auto it = deliveries.find(id);
        return it != deliveries.end() ? &it->second : nullptr;
    }

    bool hasDependencies(int id) const {
        auto it = deliveries.find(id);
        return it != deliveries.end() && !it->second.dependencies.empty();
    }

    const vector<int>& getDependencies(int id) const {
        static vector<int> empty;
        auto it = deliveries.find(id);
        return it != deliveries.end() ? it->second.dependencies : empty;
    }
};

#endif
