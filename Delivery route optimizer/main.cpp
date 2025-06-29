 #include "graph.h"
#include "delivery.h"
#include "planner.h"
#include "utils.h"

#include <iostream>
using namespace std;

void inputDeliveries(DeliveryManager& dm) {
    int d;
    cout << "Enter number of delivery packages: ";
    cin >> d;

    cout << "Enter each delivery as: id source destination deadline budget dep_count [dep1 dep2 ...]\n";
    for (int i = 0; i < d; ++i) {
        int id, src, dst, ddl, bud, depCount;
        cin >> id >> src >> dst >> ddl >> bud >> depCount;
        vector<int> deps(depCount);
        for (int j = 0; j < depCount; ++j) cin >> deps[j];
        dm.addDelivery(id, src, dst, ddl, bud, deps);
    }
}

int main() {
    Graph g;
    DeliveryManager dm;
    int cityCount = 0;

    while (true) {
        cout << "\nSmart Delivery Route Optimizer\n";
        cout << "1. Add cities and roads\n";
        cout << "2. Add delivery packages\n";
        cout << "3. Optimize delivery route\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "Enter number of cities: ";
            cin >> cityCount;

            cout << "Add roads (from to time cost), enter -1 to stop:\n";
            while (true) {
                int u;
                cin >> u;
                if (u == -1) break;
                int v, t, c;
                cin >> v >> t >> c;
                g.addEdge(u, v, t, c);
            }

        } else if (choice == 2) {
            inputDeliveries(dm);

        } else if (choice == 3) {
            if (cityCount == 0 || dm.getAllDeliveries().empty()) {
                cout << "Please input graph and deliveries first.\n";
                continue;
            }

            int start;
            cout << "Enter starting city: ";
            cin >> start;

            DeliveryPlanner planner(g, dm, cityCount);
            RouteResult result = planner.planRoute(start);

            if (!result.feasible) {
                cout << "No feasible delivery route found.\n";
            } else {
                cout << "Optimal Delivery Route:\n";
                for (int id : result.deliveryOrder) {
                    cout << "  Delivery ID: " << id << "\n";
                }
                cout << "Total Cost: " << result.totalCost << "\n";
                cout << "Total Time: " << result.totalTime << "\n";
            }

        } else if (choice == 4) {
            break;

        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
