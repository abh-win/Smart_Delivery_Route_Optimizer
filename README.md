 # Smart Delivery Route Optimizer

A C++ simulation that computes an optimal delivery route across a city network, minimizing **cost** and **time** while respecting:

- Delivery **deadlines**
- **Budget** constraints
- **Package dependencies**
- **City traffic conditions**

Built using advanced data structures and algorithms like Dijkstra’s shortest path, topological sort, and subset dynamic programming.

---

## Features

- Multi-package delivery scheduling
- Dependency-aware delivery ordering
- Deadline-constrained planning
- Budget-limited routing
- Bitmask-based subset DP for optimal route computation
- Graph representation of city roads with time & cost weights
- Modular: `Graph`, `DeliveryManager`, `Planner`, `Utils`

---

## Sample Input Format (CLI)

```
1. Add cities and roads
2. Add delivery packages
3. Optimize delivery route
4. Exit
```

### Sample Graph Input:
```
Enter number of cities: 5
Add roads (from to time cost), enter -1 to stop:
0 1 2 10
1 2 2 15
0 3 5 20
3 4 1 5
-1
```

### Sample Delivery Input:
```
Enter number of delivery packages: 2
Enter each delivery as:
id source destination deadline budget dep_count [dep1 dep2 ...]
1 0 2 10 50 0
2 2 4 20 40 1 1
```

### Sample Output:
```
Optimal Delivery Route:
  Delivery ID: 1
  Delivery ID: 2
Total Cost: 50
Total Time: 9
```

---

## How It Works

- Dijkstra is used to precompute shortest paths (time & cost) between all city pairs.
- Delivery dependencies are sorted topologically (via Kahn’s algorithm).
- A DP state `(mask, currentCity)` is used to store minimal time/cost to deliver a set of packages.
- Only feasible deliveries (based on budget, deadline, dependency) are considered at each step.

---

## 📁 Code Structure

| File | Purpose |
|------|---------|
| `main.cpp` | CLI driver program |
| `graph.h` | City graph and Dijkstra implementation |
| `delivery.h` | Delivery metadata & dependency management |
| `utils.h` | Topological sorting for delivery dependencies |
| `planner.h` | Subset DP-based delivery scheduler |

---

## Requirements

- C++17 or above
- G++ compiler

---

## Compile & Run

```bash
g++ -std=c++17 main.cpp -o optimizer
./optimizer
```

---

## Author

**Abhinav Jha**
