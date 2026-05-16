# External Sort

Sorts a file of `(url, visit_count)` pairs that doesn't fit in RAM. Two-phase algorithm: generate sorted runs with Quicksort, then merge them with a max-heap.

---

## Build & run

```bash
make   # binary → bin/
```

```bash
./bin/tp -i input.txt -o output.txt -r <entities_per_run> -f <num_tapes>
```

| Flag | Meaning |
|---|---|
| `-i` | Input file |
| `-o` | Output file |
| `-r` | Max entities loaded into RAM per run |
| `-f` | Number of tape files used during merge |

---

## Input / output format

One entity per line: `<url> <visit_count>`

**Input:**
```
http://www.google.com 3
http://www.uol.com.br 5
http://aeds.dcc.ufmg.br 6
http://www.dcc.ufmg.br 1
http://www.dcc.ufmg.br/cursos 2
```

**Output** — sorted descending by visits, ties broken alphabetically by URL:
```
http://aeds.dcc.ufmg.br 6
http://www.uol.com.br 5
http://www.google.com 3
http://www.dcc.ufmg.br/cursos 2
http://www.dcc.ufmg.br 1
```

No trailing spaces. Every line ends with `\n`.

---

## Algorithm

### Phase 1 — generate runs

Read `NUM_ENTITIES` records at a time, sort in memory with **Quicksort**, write to `rodada-N.txt`. Repeat until input is exhausted.

```
while not EOF(input):
    entities = read(NUM_ENTITIES)
    quicksort(entities)        // pivot choice must be documented
    write(entities, "rodada-{n}.txt")
    n++
```

### Phase 2 — merge runs

Open all run files simultaneously. Use a **max-heap** seeded with the first record from each run. On each iteration: pop the max, write it to output, read the next record from the same run file and push it into the heap.

```
heap = MaxHeap()
for each run file:
    heap.push(run.read(), origin=run_id)

while not heap.empty():
    (entity, run_id) = heap.pop()
    output.write(entity)
    if not EOF(run_files[run_id]):
        heap.push(run_files[run_id].read(), origin=run_id)
```

### Merge example (4 runs, 6 entities/run)

Initial state after seeding heap with first element of each run:
```
Heap: [29, 16, 14, 8]    (one from each run)
Output: []
```
Pop 29 (from F4), read next from F4 (18):
```
Heap: [18, 16, 14, 8]
Output: [29]
```
Pop 18, read next from F4 (14):
```
Heap: [16, 14, 14, 8]
Output: [29, 18]
```
...and so on until heap is empty.

---

## Complexity

| Phase | Time | Notes |
|---|---|---|
| Run generation | O(N log K) | N = total entities, K = entities per run |
| Merge | O(N log F) | F = number of run files |
| **Total** | **O(N log K + N log F)** | |

Space: O(K) for in-memory sort + O(F) for the heap.

---

## Implementation constraints

- Language: C or C++
- No STL containers (`vector`, `list`, `stack`, `queue`, etc.)
- `string` type allowed; using it to simulate data structures is not
- Implement Quicksort and the max-heap from scratch

### Optional (+2 points)
- Implement Mergesort and Heapsort as alternative sorting algorithms for phase 1; compare performance and memory access patterns experimentally
- Implement iterative (non-recursive) versions of Quicksort and Mergesort; compare recursive vs. iterative

---

## Project structure

```
TP/
├── src/        # *.c / *.cpp / *.cc
├── include/    # *.h
├── bin/        # compiled binary (generated)
├── obj/        # object files (generated)
└── Makefile
```

`make` must produce the binary in `bin/` and `.o` files in `obj/`. Don't change the directory layout.