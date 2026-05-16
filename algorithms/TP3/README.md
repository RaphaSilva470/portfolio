# Largest Fitting Rectangle

Given a grid floor plan and a list of tables (rectangles), find the largest-area table that fits inside any clear rectangular region of the floor plan. Tables can be rotated 90°.

---

## Problem

**Input:** An `N×M` grid where `.` = free space and `#` = occupied (wall or furniture), plus K table dimensions `(C, L)`.  
**Output:** The dimensions of the largest-area table that fits. On area ties, print the one with the greater width.

### What "fits" means

A table `C×L` fits if there exists a fully clear (all `.`) rectangular subregion in the grid that is at least `C` rows by `L` columns — or `L` rows by `C` columns (rotation allowed). The table doesn't need to align with any room boundary; it just needs a clear rectangle of that size anywhere on the map.

---

## Input Format

```
N M
<N lines of M characters, each '.' or '#'>
K
C1 L1
C2 L2
...
CK LK
```

### Constraints

| Variable | Range |
|---|---|
| N, M | 1 ≤ N, M ≤ 1000 |
| K | 1 ≤ K ≤ 10⁶ |
| Ci, Li | 1 ≤ Ci, Li ≤ min(500, max(N, M)) |

---

## Output Format

One line with two integers: the dimensions of the winning table.

```
C L
```

If two tables have the same area, print the one with the larger second dimension (width). It's guaranteed at least one table fits.

---

## Examples

**Input:**
```
6 16
################
#..#...........#
#......#.......#
#......#.......#
#......#....####
#########..#####
4
3 3
4 6
15 2
2 7
```

**Output:**
```
2 7
```

`3×3` fits (area 9), `2×7` fits (area 14) → `2×7` wins. `4×6` and `15×2` don't fit anywhere.

---

**Input:**
```
13 32
################################
#......##............##........#
#......##............##........#
#......##............##........#
#......##....################..#
#..######.....#................#
#..............................#
#..######.............###......#
#......####...........###......#
#......##.....#.......###.....##
#......##.....#...............##
#......##.....#............#####
#################.##############
4
3 3
3 6
15 2
7 7
```

**Output:**
```
7 7
```

---

## Implementation notes

### Core subproblem: largest rectangle in a histogram

The standard approach is a two-pass algorithm:

**Step 1 — build a height matrix.**  
For each cell `(i, j)`, compute `height[i][j]` = the number of consecutive `.` cells directly above (and including) `(i, j)`. If `grid[i][j] == '#'`, height is 0.

```
for each row i:
    for each col j:
        height[i][j] = (grid[i][j] == '.') ? height[i-1][j] + 1 : 0
```

**Step 2 — largest rectangle in each row's histogram.**  
For each row, `height[i]` is a histogram. Run the classic **stack-based O(N) algorithm** to find the largest rectangle in that histogram. This gives you the largest clear rectangle whose bottom edge is at row `i`.

Take the max across all rows → you have the largest free rectangle in the whole grid.

But you don't need just one rectangle. You need to answer K queries efficiently.

### Answering K queries

Rather than checking each table against the grid from scratch, precompute a lookup structure:

**Precompute `max_width[h]`** = the maximum width of any clear rectangle of height exactly `h` anywhere in the grid. Then `max_width[h]` answers "what's the widest rectangle I can fit if my table is `h` rows tall?"

Build it like this:
1. For every row, run the histogram algorithm, but instead of tracking just the global max, record for each height `h` the maximum rectangle width achievable.
2. A rectangle of height `h` and width `w` also means a rectangle of height `h-1` and width `w` exists — so do a suffix-max pass: `max_width[h] = max(max_width[h], max_width[h+1])`.

**Answering a query `(C, L)`:**
- Try orientation 1: table needs `C` rows, `L` cols → fits if `max_width[C] >= L`
- Try orientation 2: table needs `L` rows, `C` cols → fits if `max_width[L] >= C`

Each query is O(1) after O(N·M) preprocessing.

### Complexity

| Phase | Cost |
|---|---|
| Height matrix | O(N·M) |
| Histogram pass (all rows) | O(N·M) |
| Suffix-max on max_width | O(max(N,M)) |
| K queries | O(K) |
| **Total** | **O(N·M + K)** |

This comfortably handles 1000×1000 grids and 10⁶ queries within 5 seconds.

### Tiebreaker

Among all fitting tables, pick the one with maximum area. On area tie, pick the one with greater `L` (second dimension as printed). Sort or scan candidates accordingly.

---

## Build & run

```bash
g++ main.cpp -O2 -o tp03
./tp03 < input.txt
```

Input via stdin, output to stdout. Time limit: **5 seconds**.