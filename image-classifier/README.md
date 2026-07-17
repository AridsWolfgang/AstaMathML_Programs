# Image Classifier

> Two image classifiers built from scratch in Python — **K‑Nearest Neighbors** and a **2‑layer Neural Network** — trained on synthetic 8×8 shape images (circle, square, triangle, cross) with real forward/backward propagation.

## Architecture

```
image-classifier/
├── main.py              # Interactive REPL (train, compare, classify)
├── knn.py               # K-Nearest Neighbors (Euclidean distance)
├── nn.py                # 2-layer MLP (sigmoid, backprop from scratch)
├── features.py          # Pixel extraction, edge detection, symmetry, density
├── metrics.py           # Accuracy, confusion matrix, precision/recall/F1
├── data/
│   └── dataset.py       # Synthetic 8×8 shape generator (circle, square, triangle, cross)
└── README.md
```

## Dataset

8×8 binary images of four geometric shapes, augmented with Gaussian noise:

| Shape | Description |
|-------|-------------|
| Circle | 1px thick ring centered in the 8×8 grid |
| Square | 1px thick square border centered in the grid |
| Triangle | 1px thick isosceles triangle outline |
| Cross | Vertical + horizontal bars crossing at center |

Each sample: 64 raw pixel values + 64 edge-magnitude features + 2 symmetry scores + 1 density score = 131 features.

## Algorithms

### 1. K-Nearest Neighbors (`knn.py`)

Non-parametric classifier. For a test point **x**:

1. Compute Euclidean distance to every training point.
2. Pick the **k** closest neighbours.
3. Majority vote determines the class.

```
d(a,b) = sqrt(Σ(ai − bi)²)
```

- **No training** — just stores the data (lazy learner).
- Predictions get slower as dataset grows (O(n) per query).
- Optimal **k** is found by testing 1, 3, 5, 7.

### 2. 2-Layer Neural Network (`nn.py`)

A fully-connected MLP with one hidden layer:

```
x → W1 → sigmoid → h → W2 → sigmoid → y
```

**Forward pass:**
```
h_j = σ(Σ W1[i,j] · x_i + b1_j)
y_k = σ(Σ W2[j,k] · h_j + b2_k)
```

**Backward pass** (stochastic gradient descent):
```
δ_k = y_k − target_k
δ_j = Σ(δ_k · W2[j,k]) · σ'(h_j)
W2[j,k] ← W2[j,k] − α · δ_k · h_j
W1[i,j] ← W1[i,j] − α · δ_j · x_i
```

- Loss: binary cross-entropy per output neuron.
- Weight initialisation: Xavier (scaled by `1/√(fan_in)`).
- Learning rate: 0.5, 200 epochs max.

## Running

```bash
cd image-classifier
python main.py
```

Menu:
1. **View samples** — see one of each shape (clean, no noise)
2. **Evaluate KNN** — test k=1,3,5,7 with accuracy + per-class metrics
3. **Train Neural Network** — 200 epochs, accuracy + sample predictions
4. **Compare models** — side-by-side accuracy and training time
5. **Classify a generated image** — generate a shape with custom noise and classify with both models

## Results

| Method | Accuracy | Training Time |
|--------|----------|---------------|
| KNN (k=3) | 1.0000 | 0.001s |
| Neural Network (16 hidden) | 1.0000 | ~2s |

Both achieve perfect accuracy on the 8×8 shape dataset. The real differences appear with more complex data (natural images, higher resolution).

## Lessons

1. **KNN is a strong baseline** — on simple, low-dimensional data, KNN with enough neighbours is hard to beat. No training, no hyperparameters (except k).
2. **Neural networks need enough capacity** — 16 hidden units is plenty for 8×8 shapes (64 inputs → 16 → 4 outputs = 1,124 parameters). Too few hidden units and the network can't separate the classes.
3. **Feature engineering helps both** — edge features capture shape boundaries directly; symmetry features separate squares (high symmetry) from triangles (low symmetry).
4. **Backprop is just the chain rule** — the backward pass is mechanical: compute error at output, propagate gradients backward through each weight.
5. **Synthetic data is a good test bed** — you control the noise, the class balance, and the difficulty. Perfect for debugging before moving to real images.

## License

Educational use.
