# URL Scheduler (Depth-First Crawler)

A depth-first URL scheduler: maintains a queue of known hosts, each with a priority-sorted list of URLs. Reads commands from a file and writes output to `<input>-out.txt`.

---

## Build & run

```bash
make
./bin/tp coleta1.txt   # output → coleta1-out.txt
```

---

## Data model

```
[host queue, in discovery order]
  host1.com  →  [url_depth_1, url_depth_1, url_depth_2, ...]
  host2.net  →  [url_depth_0, url_depth_2, ...]
  ...
```

- **Host queue:** ordered by discovery time (FIFO)
- **URL list per host:** ordered by depth (ascending). Same depth → discovery order (known-first)
- Depth = number of `/` in the URL path

---

## URL normalization rules

Applied before inserting any URL:

| Rule | Example |
|---|---|
| Only HTTP allowed | `ftp://...` → discard |
| Strip trailing slash | `http://ufmg.br/` → `http://ufmg.br` |
| Strip `www.` | `http://www.ufmg.br` → `http://ufmg.br` |
| Strip `#fragment` | `http://x.com/p#sec` → `http://x.com/p` |
| Discard non-HTML files | `.jpg .gif .mp3 .avi .doc .pdf` → discard |
| No duplicates in memory | skip if URL already in the host's list |

---

## Commands

| Command | Behavior |
|---|---|
| `ADD_URLS <n>` | Read the next `n` lines as URLs, normalize and insert |
| `ESCALONA_TUDO` | Schedule (print + remove) all URLs, depth-first across all hosts |
| `ESCALONA <n>` | Schedule up to `n` URLs |
| `ESCALONA_HOST <host> <n>` | Schedule up to `n` URLs from a specific host |
| `VER_HOST <host>` | Print all URLs for the host in priority order (no removal) |
| `LISTA_HOSTS` | Print all known hosts in discovery order |
| `LIMPA_HOST <host>` | Remove all URLs for the host (keep host in queue) |
| `LIMPA_TUDO` | Remove all URLs and all hosts |

---

## Example

**Input (`coleta1.txt`):**
```
ADD_URLS 5
http://www.globoesporte.com/page1.js?q=copa#group1
ftp://ftp.globo.com
http://www.globo.com/
http://www.uol.com.br/
http://www.superdownloads.com.br/
ESCALONA 2
LISTA_HOSTS
ADD_URLS 4
http://www.globo.com/videos/jornalnacional
http://globo.com/
http://globoesporte.com/spfc/index.html
http://globo.com/videos/
ESCALONA_HOST globo.com 2
VER_HOST globo.com
```

**Output (`coleta1-out.txt`):**
```
http://globoesporte.com/page1.js?q=copa
http://globo.com
globoesporte.com
globo.com
uol.com.br
superdownloads.com.br
http://globo.com
http://globo.com/videos
http://globo.com/videos/jornalnacional
```

**What happened:**
- `ftp://ftp.globo.com` discarded (not HTTP)
- `http://www.globo.com/` normalized to `http://globo.com` (strip `www.`, strip trailing `/`)
- `ESCALONA 2` pops the two highest-priority URLs depth-first: `globoesporte.com` first (discovered first), `globo.com` second
- `globo.com/` added again after normalization — duplicate of existing `http://globo.com`, skipped
- `VER_HOST globo.com` shows remaining URLs sorted by depth

---

## Implementation constraints

- Language: C or C++
- No STL containers (`vector`, `list`, `stack`, `queue`, etc.)
- `string` type is allowed; using it to simulate data structures is not
- Implement your queue and linked list from scratch

### Optional (+2 points)

- **Breadth-first (+1):** schedule one URL per host per round, cycling through all hosts before returning to any one
- **Best-first (+1):** always pick from the host with the most URLs

---

## Project structure

```
TP/
├── src/        # *.c / *.cpp / *.cc
├── include/    # *.h
├── bin/        # binary (generated)
├── obj/        # object files (generated)
└── Makefile
```