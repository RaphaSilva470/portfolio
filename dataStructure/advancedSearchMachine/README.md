# Vector Space Model Search Engine

An in-memory inverted index + query processor using TF-IDF and cosine similarity (Zobel et al. 2006 variant). Returns the top-10 most relevant documents for a query.

---

## Build & run

```bash
make
./buscar -i consulta1.txt -o ranking1.txt -c corpus/ -s stopwords.txt
```

| Flag | Argument |
|---|---|
| `-i` | Query file (first line = query string) |
| `-o` | Output file for the ranked doc IDs |
| `-c` | Corpus directory — each file is a document, filename = document ID |
| `-s` | Stopwords file, one word per line |

Output is a single line with the top-10 document IDs, space-separated, ordered by descending similarity. Ties broken by ascending document ID.

---

## How it works

### Indexing

Build an inverted index from the corpus:

```
index[term] → [(doc_id, term_freq), ...]
```

- Strip punctuation, lowercase everything
- Skip stopwords
- Precompute `Wd` for each document at index time (avoids re-scanning on every query)

**`Wd` (document norm):**
```
Wd = sqrt(number of distinct terms in document)
```

### Scoring

Term weight in a document:
```
w(t, d) = freq(t, d) * ln(|D| / docs_containing_t)
```

Query term weight (`w(t, q)`): binary — 1 if the term appears in the query, 0 otherwise.

Similarity (Zobel 2006 variant):
```
sim(d, q) = Σ w(t,d) * w(t,q)  /  Wd
```
Query-side normalization is omitted (constant across all documents, doesn't affect ranking).

### Multi-term queries

Operator is OR. For each query term, accumulate `w(t,d) * w(t,q)` into the document's score. Documents matching more query terms naturally score higher.

---

## Worked example

Corpus (`|D| = 6`):

```
1: casa a casa da casa
2: a lua
3: casa casa maca maca maca maca maca lua
4: a maca
5: lua e lua
6: maca
```

Inverted index (stopwords: a, da, e):
```
casa → (1, 3), (3, 2)
maca → (3, 5), (4, 1), (6, 1)
lua  → (2, 1), (3, 1), (5, 2)
```

TF-IDF weights (`ln` = natural log):

| term | doc 1 | doc 2 | doc 3 | doc 4 | doc 5 | doc 6 |
|---|---|---|---|---|---|---|
| casa | 3·ln(3)=3.295 | 0 | 2·ln(3)=2.197 | 0 | 0 | 0 |
| maca | 0 | 0 | 5·ln(2)=3.465 | ln(2)=0.693 | 0 | ln(2)=0.693 |
| lua | 0 | ln(2)=0.693 | ln(2)=0.693 | 0 | 2·ln(2)=1.386 | 0 |

Document norms:
```
Wd: doc1=3.295  doc2=0.693  doc3=4.160  doc4=0.693  doc5=1.386  doc6=0.693
```

Query results:

| Query | Ranking |
|---|---|
| `casa maca` | 3, 1, 4, 6 |
| `maca lua` | 2, 4, 5, 6, 3 |
| `casa` | 1, 3 |

---

## Pseudocode

### Build index

```
for each document d in corpus:
    count term frequencies → vocab[term] = freq
    for each term in vocab:
        if term in stopwords: skip
        index[term].append((d.id, freq))
    Wd[d.id] = sqrt(distinct_terms_in_d)
```

### Query

```
scores = {}
for each term t in query:
    if t not in index: continue
    idf = ln(|D| / len(index[t]))
    w_tq = 1 * idf                        // binary query weight * idf
    for (doc_id, tf) in index[t]:
        w_td = tf * idf
        scores[doc_id] += w_tq * w_td

for doc_id in scores:
    scores[doc_id] /= Wd[doc_id]

return top_10(scores, tie_break=ascending_id)
```

---

## Implementation notes

- The inverted index must be backed by a **hash table** (not a BST/sorted structure).
- Precompute `Wd` and IDF at index time — don't recompute per query.
- Punctuation stripping and lowercasing happen at tokenization time, for both corpus and query.
- All text is English; no accent handling needed.

---

## Complexity

| Operation | Cost |
|---|---|
| Indexing | O(T) where T = total tokens in corpus |
| Single query of k terms | O(k · avg_postings_per_term + D·log D) |
| Top-10 selection | O(D) with partial sort or heap |