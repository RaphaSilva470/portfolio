# Maximum Bottleneck Path

Find the maximum weight a single truck can carry from city `u` to city `v`, given that each road has a weight limit. Classic **Widest Path / MaxiMin** problem.

Formally: given a path `P = [p1, p2, ..., pk]`, its bottleneck is `min(w(pi, pi+1))`. You want `max over all paths of min(w(pi, pi+1))`.

---

## Problem

**Input:** A weighted directed graph `G(V, E)` representing the road network + Q queries `(u, v)`.  
**Output:** For each query, the maximum load a single truck can carry from `u` to `v`.

---

## Notation

| Symbol | Meaning |
|--------|---------|
| `V`, `E` | vertex and edge sets |
| `\|V\|`, `\|E\|` | number of vertices/edges |
| `Q` | number of queries |
| `w(i, j)` | capacity of edge `(i, j)` |
| `w(P)` | bottleneck of path `P` = min of its edge weights |
| `B` | maximum edge weight in the graph |

---

## What doesn't work

- **Max Flow** — we need a single path, not flow across multiple paths. No clean modification makes max-flow return the optimal single-path result.
- **Plain BFS/DFS** — finds *a* path, not the *widest* one.

---

## Approaches

### 1. Brute Force — O(|V| · |V|!) per query

Enumerate every possible path from `u` to `v`, track the max bottleneck.

```
generate_all_paths(current_path):
    if connects(u, v, current_path):
        return bottleneck(current_path)
    i = last element in current_path
    ans = 0
    for j in adj(i):
        if j not in current_path:
            ans = max(ans, generate_all_paths(current_path + {j}))
    return ans
```

Useful for generating expected outputs on small test cases. Don't submit this.  
**Result:** 60% accuracy, TLE.

---

### 2. Brute Force + Branch and Bound — O(|V| · |V|!) per query (faster in practice)

Same as above, but prune any branch whose current edge is weaker than the best bottleneck found so far.

```
generate_some_paths(current_path):
    if connects(u, v, current_path):
        return bottleneck(current_path)
    i = last element in current_path
    ans = 0
    for j in adj(i):
        if w(i, j) > ans and j not in current_path:
            ans = max(ans, generate_some_paths(current_path + {j}))
    return ans
```

Still exponential worst-case (when the lightest edges all connect to `v`), but works on sparse graphs.  
**Result:** 100% accuracy.

---

### 3. Iterative Edge Removal — O(|E| · (|V| + |E|)) per query

Any path containing edge `(i, j)` has bottleneck ≤ `w(i, j)`. So once we've evaluated `(i, j)` as a candidate bottleneck, we can remove it from the graph entirely.

```
E' = E
ans = 0
while path_exists(u, v, E'):
    P = any_path(u, v, E')       # BFS or DFS, we don't care which path
    (i, j) = bottleneck_edge(P)
    ans = max(ans, w(i, j))
    E' = E' - {(i, j)}
return ans
```

The `while` runs at most `|E|` times; each iteration costs `O(|V| + |E|)`.  
**Result:** 100% accuracy.

---

### 4. Modified Dijkstra — O(|E| · log|V|) per query ⭐

Replace Dijkstra's relaxation:

```
// standard Dijkstra
dist[j] = min(dist[i] + w(i,j), dist[j])

// widest path variant
cap[j] = max(min(cap[i], w(i,j)), cap[j])
```

Use a **max-priority queue** (instead of min). Initialize `cap[source] = ∞`, all others = 0.

**Why it's correct:** Same inductive argument as standard Dijkstra. Suppose some better path to `j` exists outside the visited set — then the first unvisited vertex on that path would already have a higher `cap` value and would have been popped first. Contradiction.

**Variant — precompute all pairs:** Run from every vertex, store in a `|V|×|V|` matrix. Queries become O(1).  
Total cost: `O(|V| · |E| · log|V| + Q)`.

**Result:** 100% accuracy. Most common submission (~58 out of all solutions).

---

### 5. Modified Floyd-Warshall — O(|V|³) preprocessing + O(1) per query

Best option when Q is large. Replace the standard relaxation with:

```
for k in V:
    for i in V:
        for j in V:
            w[i][j] = max(w[i][j], min(w[i][k], w[k][j]))
```

After `|V|` iterations `w[i][j]` holds the max bottleneck for every pair. All queries are O(1).  
**Result:** 100% accuracy (no one submitted this, but it's the cleanest all-pairs solution).

---

### 6. Binary Search + DFS — O(log(B) · (|V| + |E|)) per query

Define `f(u, v, C) = 1` if there's a path from `u` to `v` using only edges with weight ≥ C. This is monotonically decreasing in C, so binary search works.

```
binary_search(u, v, Cleft, Cright):
    if Cleft == Cright: return Cleft
    mid = (Cleft + Cright) / 2
    if path_exists_with_min_weight(u, v, mid):   # DFS/BFS filtering edges < mid
        return binary_search(u, v, mid, Cright)
    else:
        return binary_search(u, v, Cleft, mid)
```

Initialize with `Cleft = 1`, `Cright = max_edge_weight`.  
**Result:** 100% accuracy.

---

### 7. Maximum Spanning Tree + DFS — undirected graphs only

**Key property:** In an undirected graph, the widest path between any `u` and `v` uses only edges present in the Maximum Spanning Tree.

*Proof sketch:* If the bottleneck edge of the optimal path weren't in the MST, there would be an alternative path between its endpoints using only heavier edges (by MST definition) — giving a wider path. Contradiction.

Algorithm: build the Maximum Spanning Tree (Kruskal or Prim with inverted weights), then DFS on the tree for each query. Since it's a tree, there's exactly one path between any two nodes.

```
T = maximum_spanning_tree(G)
for each query (u, v):
    return bottleneck_on_tree_path(T, u, v)   # simple DFS
```

Preprocessing: `O(|E| · log|V|)`. Per query: `O(|V| + |E|)`.  
**Note:** does not apply to directed graphs.

---

## Complexity summary

| Approach | Preprocessing | Per query | Good when |
|---|---|---|---|
| Brute force | — | O(\|V\| · \|V\|!) | Never in prod; useful for test generation |
| Branch & bound | — | O(\|V\| · \|V\|!) | Sparse graphs only |
| Edge removal | — | O(\|E\| · (\|V\|+\|E\|)) | — |
| Modified Dijkstra | — | O(\|E\| · log\|V\|) | Default choice |
| Dijkstra all-pairs | O(\|V\| · \|E\| · log\|V\|) | O(1) | Q is large |
| Floyd-Warshall | O(\|V\|³) | O(1) | Dense graph + large Q |
| Binary search + DFS | — | O(log(B) · (\|V\|+\|E\|)) | B is small |
| MST + DFS | O(\|E\| · log\|V\|) | O(\|V\|+\|E\|) | Undirected + large Q |

---

## Build & run

```bash
make
./tp02 < input.txt
```

Input via stdin, output to stdout. Time limit: **5 seconds**.

---

## Common mistakes

**Forgetting Q in your complexity.** `O(|E| log|V|)` (one Dijkstra run) vs `O(Q · |E| log|V|)` (one per query) is a massive difference. Every value in the input is a variable, not a constant.

**"It works on the examples" is not a correctness proof.** Use induction or contradiction. Examples show a solution *can* work; they don't show it *always* works.

**Max flow.** The single-truck constraint makes this the wrong tool entirely.

**Unnamed variables in complexity.** `O(n²)` without defining `n` says nothing about your algorithm.