# Neural Network Hyperparameter Grid Search

Grid search over batch size, learning rate, and hidden layer size on a 28×28 image classification task. 36 combinations trained and compared, with convergence curves broken down by each hyperparameter axis.

---

## What it does

Trains a simple feedforward network across all combinations of:

| Hyperparameter | Values tested |
|---|---|
| Hidden units | 25, 50, 100 |
| Learning rate | 0.5, 1, 10 |
| Batch size | 1, 10, 50, full batch |

Optimizer fixed to SGD. Loss: categorical crossentropy. Early stopping on val_loss (patience=5). Max 20 epochs per run.

Results are plotted grouped by each hyperparameter axis so you can isolate its effect on convergence.

---

## Architecture

```
Input (784,)
  → Dense(hidden_units, sigmoid)
  → Dense(10, softmax)
```

Single hidden layer. Output is 10-class softmax (image classification).

---

## Key findings

**Batch size** has the biggest visible effect on training stability. Smaller batches (1, 10) produce noisy loss curves; larger batches converge more smoothly. Full-batch GD gives the cleanest curves but is costlier per epoch. Mini-batch is the practical middle ground.

**Learning rate** is the most dangerous knob. LR=10 causes divergence across all batch sizes — models converge to near-random accuracy. LR=0.5 works well for small batches; LR=1 works better for larger batches where gradient estimates are more stable.

**Hidden layer size** had minimal impact beyond 25 units for this dataset. 25 → 50 → 100 neurons showed no meaningful improvement, suggesting the task doesn't require more representational capacity.

---

## Stack

```
Python · TensorFlow/Keras · NumPy · Pandas · Matplotlib · scikit-learn
```

---

## Setup

```bash
pip install tensorflow numpy pandas matplotlib scikit-learn

# dataset is downloaded from Google Drive in the notebook
# run:
jupyter notebook hyperparameter_search.ipynb
```

Data is a CSV of 28×28 grayscale images (784 pixel values + 1 label column). Normalized to [0, 1], split 80/20 train/test.