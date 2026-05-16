# AdaBoost from Scratch — Tic-Tac-Toe

AdaBoost implemented from scratch using decision stumps, applied to the [Tic-Tac-Toe Endgame dataset](https://archive.ics.uci.edu/dataset/101/tic+tac+toe+endgame) (UCI). No sklearn estimators used for the model itself.

---

## What it does

Binary classification: given a tic-tac-toe board state (9 cells, each `x`/`o`/blank), predict whether the first player (x) wins.

The model combines up to 300 decision stumps sequentially. Each stump is trained on the residual errors of the previous ones, with sample weights updated via the AdaBoost weight formula.

---

## Implementation

Two classes, built from scratch:

**`DecisionStump`** — a single-feature threshold classifier (the "weak learner"):
- `fit(X, y, sample_weight)`: iterates over all features and thresholds, picks the split that minimizes weighted error. Flips polarity if error > 0.5 (so every stump does at least marginally better than chance).
- `predict(X)`: applies the learned threshold and polarity.

**`SimpleBoosting`** — the AdaBoost loop:
- `fit(X, y)`: trains `n_estimators` stumps sequentially. After each stump, computes `alpha = 0.5 * log((1 - err) / err)` and updates sample weights exponentially so misclassified samples get more focus next round.
- `predict(X)`: returns `sign(Σ alpha_i * stump_i(x))` — weighted vote across all stumps.

---

## Results

Test error evaluated with 5-fold cross-validation across 1–300 stumps:

- Error drops sharply in the first ~50 stumps
- Stabilizes around 200 stumps with no significant further improvement
- Final model is robust on this dataset

---

## Stack

```
Python · NumPy · Pandas · Matplotlib · scikit-learn (KFold only) · ucimlrepo
```

---

## Setup

```bash
pip install numpy pandas matplotlib scikit-learn ucimlrepo

# run the notebook
jupyter notebook boosting.ipynb
```

Dataset is fetched automatically via `ucimlrepo` — no manual download needed.

---

## Dataset

958 instances, 9 categorical features (board positions), binary target. Features mapped to `{x: 1, o: -1, b: 0}`, labels to `{positive: 1, negative: -1}`.