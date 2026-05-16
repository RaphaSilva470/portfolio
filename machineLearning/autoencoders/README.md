# Image Autoencoder — MNIST

Two autoencoder variants trained on MNIST: one focused on reconstruction quality, one on compression ratio. Built with Keras.

---

## What it does

An autoencoder learns to compress an image into a compact latent representation and then reconstruct it. This project explores two use cases:

1. **Reconstruction** — how faithfully can the model rebuild the original image from a bottleneck of 64 dimensions?
2. **Compression** — push the bottleneck to 32 dimensions (24.5× compression ratio from 784 input pixels) and measure the quality trade-off.

---

## Models

### Reconstruction autoencoder

```
Input (28×28×1)
  → Flatten → Dense(128, relu) → Dense(64, relu)       # encoder
  → Dense(128, relu) → Dense(784, sigmoid) → Reshape   # decoder
Output (28×28×1)
```

- Loss: MSE
- Optimizer: Adam
- Latent dim: **64**

### Compression autoencoder

```
Input (28×28×1)
  → Flatten → Dense(128, relu) → Dense(64, relu) → Dense(32, relu)   # encoder
  → Dense(64, relu) → Dense(128, relu) → Dense(784, sigmoid) → Reshape  # decoder
Output (28×28×1)
```

- Loss: binary crossentropy
- Optimizer: Adam
- Latent dim: **32**
- Compression ratio: 784 / 32 = **24.5×**

Both models trained for 50 epochs, batch size 256.

---

## Stack

```
Python · Keras (TensorFlow backend) · NumPy · Matplotlib
```

---

## Setup

```bash
pip install tensorflow numpy matplotlib

# run the notebook or script
jupyter notebook autoencoder.ipynb
```

Dataset is loaded directly via `keras.datasets.mnist` — no manual download needed.

---

## Results

The reconstruction model reliably reproduces digit shapes with minimal blur at 64 latent dimensions. The compression model achieves 24.5× size reduction while keeping digits legible — degradation is visible mainly in thinner strokes and fine details.

Compression stats printed at runtime:
```
Original size:     784 dimensions
Compressed size:   32 dimensions
Compression ratio: 24.50
```