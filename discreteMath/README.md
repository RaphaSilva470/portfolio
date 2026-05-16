# Spiral Coordinate Generator

Given a point index `n`, compute its `(x, y)` coordinates on two distinct integer spirals: square and triangular. Implemented in C with O(1) closed-form solutions.

---

## The problem

Two infinite sequences of integer-coordinate points, each following a different geometric pattern. Given `n ≥ 0`, output the coordinates of the nth point without iterating through all previous points.

The interesting constraint: a naive O(n) walk is trivial. The goal is to find a mathematical pattern that gives you the answer directly — ideally O(1).

---

## Square spiral

Points walk outward in a clockwise square pattern:

```
n=0  → ( 0,  0)
n=1  → ( 0,  1)
n=2  → (-1,  1)
n=3  → (-1,  0)
n=4  → (-1, -1)
n=5  → ( 0, -1)
n=6  → ( 1, -1)
n=7  → ( 1,  0)
n=8  → ( 1,  1)
n=9  → ( 1,  2)
n=10 → ( 0,  2)
```

**Pattern:** each "ring" k contains 8k points (except ring 0). By figuring out which ring `n` belongs to and where within that ring it falls, you can compute `(x, y)` directly in O(1).

Ring k starts at point `2k² - 2k + 1` (for k ≥ 1). Within each ring there are 4 sides of length `2k`, and each side has a known direction vector and starting corner.

---

## Triangular spiral

Points walk outward along equilateral-triangle-like paths:

```
n=0  → ( 0,  0)
n=1  → ( 1,  0)
n=2  → ( 0,  1)
n=3  → (-1,  0)
n=4  → (-2, -1)
n=5  → (-1, -1)
n=6  → ( 0, -1)
n=7  → ( 1, -1)
n=8  → ( 2, -1)
n=9  → ( 3, -1)
n=10 → ( 2,  0)
```

**Pattern:** each ring k contains 3k points. Ring k starts at point `3k(k-1)/2 + 1`. Each ring has 3 sides of varying step directions, which you can derive from the triangular geometry.

---

## Build & run

```bash
gcc -o espquadrada espquadrada.c
gcc -o esptriangular esptriangular.c

echo "10" | ./espquadrada     # → (0, 2)
echo "10" | ./esptriangular   # → (2, 0)
```

Input: a single integer `n` on stdin.  
Output: `(x, y)` on stdout.

---

## Complexity

| Spiral | Approach | Complexity |
|---|---|---|
| Square | Closed-form ring + offset | Θ(1) |
| Triangular | Closed-form ring + offset | Θ(1) |

The O(1) solution requires finding a closed form for "which ring does point n fall in?" — which reduces to inverting a quadratic for the square spiral and a triangular number formula for the triangular one. From there, the position within the ring is a simple linear offset.

---

## Files

```
espquadrada.c     # square spiral
esptriangular.c   # triangular spiral
```