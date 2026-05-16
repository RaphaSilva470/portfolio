# Video Game Sales Analysis (1980–2016)

Exploratory analysis and classification on the [Video Games Sales dataset](https://www.kaggle.com/datasets/sidtwr/videogames-sales-dataset?resource=download&select=Video_Games_Sales_as_at_22_Dec_2016.csv) (~16k titles). Four questions investigated:

1. Does genre popularity vary by platform in a statistically significant way?
2. Did any genre have high variance in popularity over time?
3. Does genre market share differ significantly across regions?
4. Can we predict, from Genre and Publisher alone, whether a game sold more in Japan than in the rest of the world combined?

---

## Stack

```
pandas, numpy, matplotlib, seaborn, scikit-learn
```

---

## Dataset

16,719 records, columns used:

| Column | Type | Description |
|---|---|---|
| `Name` | str | Game title |
| `Platform` | str | Console |
| `Year_of_Release` | int | Release year |
| `Genre` | str | Game genre |
| `Publisher` | str | Publisher |
| `NA_Sales` | float | North America sales (millions) |
| `EU_Sales` | float | Europe sales (millions) |
| `JP_Sales` | float | Japan sales (millions) |
| `Other_Sales` | float | Rest of world sales (millions) |
| `Global_Sales` | float | Total worldwide sales (millions) |

Critic/user score columns dropped (not used). Rows with nulls dropped. Records with `Year_of_Release > 2016` dropped (partial data).

---

## Questions

### 1. Genre bias by platform?

**Method:** Bootstrap hypothesis test (99% confidence interval, 1000 samples).  
Null hypothesis: genre sales variation across platforms is explained by chance.

For each `(platform, genre)` pair, compared the bootstrap CI of sales within the platform vs. outside it. Tested on the two highest-volume genres globally: **Action** and **Sports**.

**Result:** Null rejected for all platforms. Genre sales distribution is not platform-agnostic — most platforms show Action and Sports above expected, while platforms with zero titles in those genres pull below.

---

### 2. Genre popularity variance over time?

**Method:** Sales approximated by release year (assumption: most sales occur near launch). Genre share calculated as fraction of total sales for games released in a given year.

Focused on Action and Sports (highest title count) to avoid noise from niche genres with sparse yearly releases. Plotted fraction of yearly sales per genre, per region.

**Notable findings:**
- Japan showed an earlier and steeper decline in Sports titles (~2008), with no recovery through 2016.
- `Other_Sales` had an isolated Action spike around 2004 not present in any other region.
- Area plots for all 12 genres show the full composition of yearly sales — stacked to 100% per year per region.

---

### 3. Regional genre bias?

**Method:** Permutation test (99.9% significance level, 1000 simulations).  
Null hypothesis: `p_local = p_global`, where `p` is the genre's share of total sales.

Tested Action and Sports across NA, EU, JP, Other.

**Result:** Null rejected for all combinations. Even small-looking differences are significant given the scale — minimum precision in this dataset is ~10k units, and total regional markets are in the hundreds of millions.

---

### 4. Predict Japan-majority sales?

**Target variable:** `Vendeu_Mais_Japao` = 1 if `JP_Sales > NA_Sales + EU_Sales + Other_Sales`, else 0.  
~3,800 positive cases out of ~11k total after cleaning.

**Features:** One-hot encoded `Publisher` and `Platform` columns. Sales columns excluded (would be data leakage).

**Models tested:**

| Model | F1 (class 1) | Accuracy |
|---|---|---|
| KNN (k=3) | poor | — |
| Gradient Boosting (200 trees, depth=6) | 0.74 | ~0.91 |

Gradient Boosting with `max_depth=6, n_estimators=200` was the best result. Cross-validated with StratifiedKFold (5 splits).

**Confusion matrix summary:**
- Non-Japan majority (class 0): precision 0.91, recall 0.95 — model is confident here
- Japan majority (class 1): precision 0.81, recall 0.68 — harder to catch, likely due to class imbalance

The class imbalance (~3.8k vs ~7.5k) is the main bottleneck. A larger or more balanced dataset would likely improve recall on class 1.

---

## Running

```bash
pip install pandas numpy matplotlib seaborn scikit-learn

# dataset is loaded directly from GitHub in the notebook:
# https://raw.githubusercontent.com/diogenesvazmelo/TP-Introducao_Ciencia_de_Dados/main/Database/Video_Games_Sales_as_at_22_Dec_2016.csv
```

Open and run the notebook top to bottom. No local data files needed.