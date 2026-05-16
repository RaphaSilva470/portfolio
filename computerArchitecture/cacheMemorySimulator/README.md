# Cache Memory Simulator

Simulates a single-level set-associative cache. Given a sequence of memory accesses, prints the cache state after each access. Supports direct-mapped, set-associative, and fully-associative configurations.

---

## Build & run

```bash
# C/C++
gcc -o simulador main.c
./simulador <cache_size> <line_size> <set_size> <access_file>

# Python
python3 simulador.py <cache_size> <line_size> <set_size> <access_file>
```

### Arguments

| # | Argument | Example | Meaning |
|---|---|---|---|
| 1 | `cache_size` | `4096` | Total cache size in bytes |
| 2 | `line_size` | `1024` | Size of each cache line in bytes |
| 3 | `set_size` | `4` | Lines per set (1 = direct-mapped, all lines = fully associative) |
| 4 | `access_file` | `accesses.txt` | File with hex memory addresses, one per line |

---

## Fixed parameters

- 32-bit address space
- Addresses reference bytes (not words)
- Replacement policy: **LRU** (timestamp-based)
- Lines fill within a set in order (first slot, then second, etc.)
- Lines in a set are stored consecutively in the cache
- Max 1000 cache lines

---

## Input file format

One hex address per line:

```
0xDEADBEEF
0x00000000
0x12345678
```

Read with `fscanf(f, "%x", &addr)` in C, or `int(line, 16)` in Python.

---

## Output format

Print the full cache state after **each** access, separated by `================`. No separator before the last access.

```
================
IDX V ** ADDR **
000 1 0x0037AB6F
001 0
002 0
003 0
================
IDX V ** ADDR **
000 1 0x0037AB6F
001 1 0x00000000
002 0
003 0
================
IDX V ** ADDR **
000 1 0x0037AB6F
001 1 0x00000000
002 1 0x00048D15
003 0
```

### Field rules

| Field | Format | Notes |
|---|---|---|
| `IDX` | 3-digit zero-padded integer | Line index across the whole cache, 0-based |
| `V` | `0` or `1` | Valid bit. `0` = empty line |
| `ADDR` | `0x` + 8 uppercase hex digits | Block tag. Only printed when `V = 1` |

`printf("%03d", idx)` and `printf("0x%08X", tag)` are your friends.

---

## How the address breaks down

For a 1KB line size (10 offset bits) with fully-associative mapping:

```
Original:   0xDEADBEEF
Zero offset bits (10 LSBs): 0xDEADBC00
Shift right by 10:          0x0037AB6F   ← this is what you store as ADDR
```

General formula:
```
offset_bits = log2(line_size)
set_bits    = log2(num_sets)          // num_sets = cache_size / line_size / set_size
tag         = address >> (offset_bits + set_bits)
set_index   = (address >> offset_bits) & (num_sets - 1)
```

The stored `ADDR` field is the **full tag shifted back** to 32 bits for display:
```
display_addr = tag << set_bits   // fills the tag+set portion, offset zeroed
```

Wait — actually re-read the spec: the displayed value is just `address >> offset_bits` (zero out the offset bits, shift right). Verify with the example:
```
0xDEADBEEF >> 10 = 0x0037AB6F  ✓
0x12345678 >> 10 = 0x00048D15  ✓
```

---

## Implementation sketch

```
cache_size  = total_bytes / line_size        # total number of lines
num_sets    = cache_size / set_size
offset_bits = log2(line_size)
set_bits    = log2(num_sets)

struct Line { valid, tag, timestamp }
cache[num_sets][set_size]

for each access:
    set   = (addr >> offset_bits) & (num_sets - 1)
    tag   = addr >> (offset_bits + set_bits)
    
    if tag in cache[set]:          # hit
        update timestamp
    else:                          # miss
        evict LRU line if set full
        store tag, valid=1, timestamp=now
    
    print full cache state
```

LRU: each line keeps an integer timestamp. On access, set to a global counter. On eviction, pick the line with the lowest timestamp.

---

## Delivery

Submit a single `.zip` containing:
- `simulador.c` / `simulador.py` (or equivalent)
- A `.txt` with group members' names and IDs, plus the exact compile and run commands

**Output format is graded automatically — a single wrong space or missing zero will fail the test.**