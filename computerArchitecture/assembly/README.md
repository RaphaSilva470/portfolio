# RISC-V Assembly — TP01

Two standalone RISC-V assembly procedures, implemented and tested in the [Venus/Ripes simulator](https://www.kvakil.me/venus/).

> **Note:** the simulator uses 32-bit registers, even though course material describes 64-bit. Adjust accordingly.

---

## Tooling

Use the [Venus simulator](https://www.kvakil.me/venus/): paste your `.s` file into the **Editor** tab, run it in the **Simulator** tab. Step through line by line and inspect register values.

Syntax differences from standard RISC-V: https://github.com/mortbopet/Ripes/blob/master/docs/introduction.md

---

## Problem 1 — 3n+1 Sequence (`1.s`)

Implement a procedure that returns the **n-th term** of the Collatz (3n+1) sequence starting from a given value.

### Registers

| Register | Role |
|---|---|
| `x10` | Input: which term to return (1-indexed), Output: the term's value |
| `x11` | Input: starting value of the sequence |

### Sequence rules

```
next(a) = a / 2      if a is even
next(a) = 3*a + 1    if a is odd
```

The sequence ends when it reaches 1.

**Example** — starting value `11`:
```
11, 34, 17, 52, 26, 13, 40, 20, 10, 5, 16, 8, 4, 2, 1
```
The 5th term is `26`.

If `n` exceeds the total number of terms in the sequence, return `0`. For example, the 20th term starting from `11` → `0` (sequence only has 15 terms).

---

## Problem 2 — Recursive Selection Sort (`2.s`)

Implement **recursive selection sort** in assembly.

### Registers

| Register | Role |
|---|---|
| `x10` | Pointer to the start of the array |
| `x11` | Number of elements |

### Algorithm

```
selection_sort(array, n):
    if n > 1:
        pos = index of maximum element in array[0..n-1]
        swap(array[pos], array[n-1])
        selection_sort(array, n-1)
    else:
        return
```

Use `x2` (stack pointer) to save/restore registers across recursive calls.

---

## Delivery

Submit a single `.zip` containing:
- `1.s` — Problem 1 solution
- `2.s` — Problem 2 solution
- A `.txt` with your name and student ID

Comment your code. It's worth points and makes debugging easier.