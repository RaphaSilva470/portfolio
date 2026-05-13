# 🎶 Spotify Big Data Analysis — PySpark on HDFS

Large-scale analysis of 1.6 million Spotify playlists and 10 million+ tracks using Apache Spark, running distributed on a Hadoop cluster. Three analytical tasks covering outlier detection, artist popularity trends over time, and playlist diversity patterns.

---

## Overview

The dataset sits in HDFS (~3 GB of JSON) and is queried entirely through **PySpark DataFrames** — no Pandas, no single-threaded fallbacks. Spark handles the heavy lifting; results are collected into small aggregates and visualized with matplotlib.

**Stack:** PySpark · HDFS · Hadoop · Python · matplotlib

---

## Dataset

Two JSON files in HDFS at `hdfs://localhost:9000/datasets/spotify/`:

| File | Contents | Size |
|---|---|---|
| `playlists.json` | 1.6M playlists — name, followers, track count, timestamps | ~0.5 GB |
| `tracks.json` | 10M+ track entries — artist, album, duration, playlist ID | ~2.5 GB |

Key fields used across tasks:

```
playlists.json → pid, name, modified_at, num_followers
tracks.json    → pid_playlist, track_uri, artist_name, artist_uri, duration_ms
```

---

## Task 1 — Song Duration Outlier Removal (IQR method)

**Goal:** Clean the dataset by removing statistically extreme track durations before any further analysis.

### Approach

Raw duration data includes everything from 1-second ad jingles to 3-hour DJ sets. Used the **Interquartile Range Rule (IQRR)** to define a valid range:

```
valid range: Q1 - 1.5×IQR  <  duration  <  Q3 + 1.5×IQR
             where IQR = Q3 - Q1
```

Computed entirely in Spark using `approxQuantile` on the full 10M+ row DataFrame, then filtered with a single `where()` call — no data is collected to the driver until the final summary stats.

### Results

**Before filtering:**

| Metric | Duration (ms) |
|---|---|
| Min | ~0 |
| Avg | ~245,000 |
| Max | ~10,800,000 |

**After IQRR filtering:**

| Metric | Duration (ms) |
|---|---|
| Min | ~60,000 |
| Avg | ~220,000 |
| Max | ~360,000 |

A significant number of tracks were flagged as outliers (mostly very short clips and a few extremely long recordings). The filtered dataset is used as the baseline for the remaining tasks.

---

## Task 2 — Top Artists Popularity Over Time

**Goal:** Find the 5 most-featured artists across all playlists and track how their presence evolved year by year.

### Approach

1. Join `tracks` with `playlists` on `pid` to get each track's playlist context including `modified_at`
2. Group by `artist_uri` + year (extracted from `modified_at` Unix timestamp) and count distinct playlist appearances
3. Rank artists by total playlist count → take top 5
4. Pivot to get a time series per artist and plot

```python
# Year extraction in Spark
from pyspark.sql.functions import year, from_unixtime
df.withColumn("year", year(from_unixtime("modified_at")))
```

### Output

Line chart with one line per artist (2010–2017), x-axis = year, y-axis = number of playlists containing that artist. Results show clear growth trends for top artists across the platform's lifespan, with some artists dominating early years and others rising later.

---

## Task 3 — Playlist Diversity (Artist Prevalence CDF)

**Goal:** Understand whether Spotify users tend to build artist-focused playlists or diverse ones.

### Approach

For each playlist, compute the **prevalence of the most frequent artist** — i.e., what fraction of the playlist's tracks are by a single artist:

```
artist_prevalence = max(tracks_by_artist) / total_tracks_in_playlist
```

Steps in Spark:
1. Count tracks per `(pid, artist_uri)` pair
2. For each playlist, find the max count and divide by `num_tracks`
3. Collect the resulting distribution and plot as a CDF

### Output

CDF plot with artist prevalence (0–1) on the x-axis and cumulative probability on the y-axis. The curve reveals the balance between single-artist-heavy playlists and fully mixed ones — with a large portion of playlists sitting at low prevalence values, suggesting users tend to mix artists rather than dedicate playlists to one.

---

## Project Structure

```
├── task1_outliers.py        # IQR filtering + summary stats
├── task2_popularity.py      # Top-5 artists over time + line chart
├── task3_diversity.py       # Artist prevalence per playlist + CDF plot
└── outputs/
    ├── task1_before.txt
    ├── task1_after.txt
    ├── task2_chart.png
    └── task3_cdf.png
```

---

## Running

All jobs submit to the local Spark cluster with the dataset already in HDFS:

```bash
spark-submit task1_outliers.py
spark-submit task2_popularity.py
spark-submit task3_diversity.py
```

To check HDFS dataset availability:

```bash
hdfs dfs -ls hdfs://localhost:9000/datasets/spotify/
```

---

## Notes

- All heavy computation goes through Spark's distributed execution engine — no `.toPandas()` or NumPy processing on full DataFrames
- `approxQuantile` is used for Q1/Q3 computation (exact quantiles would require a full sort across all partitions, which doesn't scale)
- Results are only `.collect()`ed after being reduced to small aggregates

---

## Context

Built as part of a graduate course on **Cloud Computing and Big Data**, focusing on distributed data processing patterns with Apache Spark on a shared Hadoop cluster.
