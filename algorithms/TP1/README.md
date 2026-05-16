# +Bikes Allocation System

Stable matching between visitors and bikes on a grid map with obstacles. Think Gale-Shapley, but the "preference" side (bikes) is determined by shortest path distance instead of explicit rankings.

---

## Problem

Given:
- `N` visitors and `N` bikes on a grid (up to 1000×1000)
- Obstacles that block movement (4-directional only)
- A preference matrix: each visitor ranks all bikes by preference score

Produce a **stable (fair) allocation** where no visitor-bike pair would both prefer each other over their current match.

### Stability criteria

An allocation is stable if, for every matched pair `(p1, b1)`:
1. If `p1` prefers some other bike `b2` over `b1`, then there's a visitor `p2` **closer** to `b2` who is already matched to `b2`.
2. If some visitor `p2` is **closer** to `b1` than `p1`, then `p2` is matched to a bike `p2` prefers over `b1`.

In short: **preference wins over distance**, but a visitor can't "steal" a bike from someone physically closer to it.

### Tiebreakers
- **Equal preference scores** → lower bike ID wins
- **Equal distance to a bike** → lower visitor ID wins

---

## Input Format

```
N
ROWS COLS
<map line 1>
...
<map line ROWS>
<preference row for visitor 1>
...
<preference row for visitor N>
```

### Map characters

| Char | Meaning |
|------|---------|
| `*` | Empty cell (walkable) |
| `-` | Obstacle (blocks movement) |
| `a`–`j` | Visitor (ID = char - 'a' + 1) |
| `0`–`9` | Bike (ID = digit + 1, so `0` = ID 1, `9` = ID 10) |

Visitors can walk through cells occupied by other visitors or bikes, but **not** through obstacles or outside the grid.

### Preference matrix

Line `i` contains `N` integers. The `j`-th value is visitor `i`'s preference score for bike `j`. Higher = more preferred.

---

## Output Format

Print `N` lines, one per visitor, sorted by visitor ID:

```
a 3
b 0
c 1
```

Each line: `<visitor_char> <bike_digit>`

---

## Example

**Input:**
```
2
4 4
a**1
--**
*-**
b*0*
3 4
4 5
```

**Output:**
```
a 1
b 0
```

**Why:** `a` prefers bike `1` (score 4 > 3), so `a` gets it. `b` would prefer bike `1` too, but `a` is closer — so `b` gets `0`.

---

## Bigger example (5 visitors)

**Input:**
```
5
4 5
c*d3a
--1*2
0-*b*
***4e
1 2 4 5 3
4 3 5 1 2
5 4 3 2 1
3 4 2 5 1
1 4 5 2 3
```

**Output:**
```
a 3
b 2
c 0
d 1
e 4
```

---

## Implementation notes

### Step 1 — Compute distances
Run a **BFS from each bike** to find shortest distances to all visitors. With up to 10 bikes and a 1000×1000 grid, that's 10 BFS runs at O(N×M) each → O(10 × N × M) total.

### Step 2 — Build proximity ranking per bike
For each bike, sort visitors by distance (tiebreak: lower visitor ID first).

### Step 3 — Run stable matching
Classic **Gale-Shapley** with visitors proposing:
- Visitors propose in decreasing preference order (tiebreak: lower bike ID first).
- A bike accepts the closest proposer it has seen so far (tiebreak: lower visitor ID).
- A displaced visitor re-proposes to their next preferred bike.

### Complexity
- BFS: `O(B × N × M)` where B ≤ 10
- Stable matching: `O(N²)` in the worst case
- Total: `O(B × N × M + N²)` — well within the 5s limit for the given constraints

---

## Build & run

```bash
make          # produces ./tp01
./tp01 < input.txt
```

---

## Constraints

- `1 ≤ N ≤ 10` (visitors and bikes)
- `1 ≤ ROWS, COLS ≤ 1000`
- Time limit: **5 seconds**
- Language: **C or C++**, standard library only