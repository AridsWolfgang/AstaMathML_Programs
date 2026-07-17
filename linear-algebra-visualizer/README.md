# Linear Algebra Visualizer

An interactive single-page HTML application for exploring fundamental concepts of linear algebra through visual, hands-on demonstrations. Open `index.html` in any modern browser — no server or internet connection required.

---

## Table of Contents

1. [Vector Spaces](#1-vector-spaces)
2. [Linear Transformations](#2-linear-transformations)
3. [3D Projection & Rotation](#3-3d-projection--rotation)
4. [Eigendecomposition](#4-eigendecomposition)

---

## 1. Vector Spaces

### What is a Vector Space?

A **vector space** over a field **F** (typically real numbers ℝ) is a set **V** equipped with two operations:

- **Vector addition**: `u + v` for any `u, v ∈ V`
- **Scalar multiplication**: `c·v` for any `c ∈ F`, `v ∈ V`

These operations must satisfy eight axioms (closure, associativity, commutativity, identity element, inverse elements, distributive properties, and scalar multiplication compatibility).

### Visualizing Vectors in ℝ²

In this application, vectors are displayed as **arrows** originating from the origin (0,0) in a 2D coordinate plane. Each vector `v = (v₁, v₂)` has:

- **Components**: `v₁` along the x-axis, `v₂` along the y-axis
- **Magnitude (length)**: `‖v‖ = √(v₁² + v₂²)`
- **Direction**: angle `θ = arctan(v₂ / v₁)`

### Vector Operations

| Operation | Formula | Visual Meaning |
|-----------|---------|----------------|
| Addition | `(a,b) + (c,d) = (a+c, b+d)` | Place tail of w at head of v |
| Subtraction | `(a,b) − (c,d) = (a−c, b−d)` | Vector from head of w to head of v |
| Scalar multiplication | `c·(a,b) = (c·a, c·b)` | Stretch (c>1), shrink (0<c<1), or reverse (c<0) |
| Dot product | `v · w = v₁w₁ + v₂w₂ = ‖v‖·‖w‖·cos(θ)` | Measures alignment — zero means perpendicular |

### Why Dot Product Matters

The dot product reveals geometric relationships:
- **v · w > 0**: vectors point in roughly the same direction
- **v · w = 0**: vectors are orthogonal (perpendicular)
- **v · w < 0**: vectors point in opposite directions

### Interactive Demo

In the **Vector Space** tab:
- Click anywhere on the canvas to place vectors `v` (red) and `w` (green)
- Drag the arrow heads to adjust components in real time
- Use the scalar multiplier slider to see `c·v` (gold)
- The output panel continuously displays coordinates, dot product, sum, and difference

---

## 2. Linear Transformations

### Definition

A **linear transformation** (or linear map) is a function `T: V → W` between vector spaces that preserves vector addition and scalar multiplication:

```
T(u + v) = T(u) + T(v)   (additivity)
T(c·v) = c·T(v)          (homogeneity)
```

### Matrix Representation

Any linear transformation from ℝ² → ℝ² can be represented by a **2×2 matrix**:

```
T(x, y) = [[a, b], [c, d]] · [x, y]ᵀ = (ax + by, cx + dy)
```

The matrix `M = [[a, b], [c, d]]` encodes how the basis vectors transform:
- `T(1, 0) = (a, c)` — first column
- `T(0, 1) = (b, d)` — second column

### The Determinant

The **determinant** of a 2×2 matrix is:

```
det(M) = a·d − b·c
```

Geometric meaning:
- `|det|` = area scaling factor (how much the transformation stretches/shrinks area)
- `det = 0` → transformation collapses space to a line or point (singular/non-invertible)
- `det < 0` → transformation flips orientation (mirror)

### Common Transformations

| Type | Matrix | Effect |
|------|--------|--------|
| Identity | `[[1,0],[0,1]]` | No change |
| Scale | `[[s,0],[0,s]]` | Uniform scaling |
| Shear (x) | `[[1,k],[0,1]]` | Slides rows |
| Rotation (θ) | `[[cosθ,−sinθ],[sinθ,cosθ]]` | Rotates by angle θ |
| Reflection (x-axis) | `[[1,0],[0,−1]]` | Flips over x-axis |

### Interactive Demo

In the **Matrix Transform** tab:
- A house shape is shown in gray (original) and red (transformed)
- Four sliders control matrix entries `a`, `b`, `c`, `d`
- The output shows `det(M)` in real time
- Click **Animate** to cycle through a sequence of transformations
- **Reset Shape** returns to the identity matrix

---

## 3. 3D Projection & Rotation

### 3D Coordinate Systems

A point in 3D space is represented by three coordinates `(x, y, z)`. We use a **right-handed coordinate system** where:
- x points right
- y points up
- z points out of the screen

### Rotation in 3D

Rotations around each axis are described by orthogonal matrices:

**Rotation about the X-axis** (roll):
```
Rx(θ) = [[1,    0,     0],
         [0,  cosθ, −sinθ],
         [0,  sinθ,  cosθ]]
```

**Rotation about the Y-axis** (pitch):
```
Ry(θ) = [[ cosθ, 0, sinθ],
         [    0, 1,    0],
         [−sinθ, 0, cosθ]]
```

**Rotation about the Z-axis** (yaw):
```
Rz(θ) = [[cosθ, −sinθ, 0],
         [sinθ,  cosθ, 0],
         [   0,     0, 1]]
```

The combined rotation is computed by matrix multiplication: `R = Rz · Ry · Rx`.

### Perspective Projection

To display 3D points on a 2D screen, we project them using a **perspective projection**:

```
x₂ = x₁ · D / (D + z₁)
y₂ = y₁ · D / (D + z₁)
```

Where:
- `(x₁, y₁, z₁)` is the 3D point after rotation
- `(x₂, y₂)` is the 2D screen coordinate
- `D` is the viewer distance (focal length)

This creates a **vanishing point** effect: objects farther away appear smaller.

### Wireframe Cube Rendering

A cube has 8 vertices `(±1, ±1, ±1)` and 12 edges. The rendering pipeline:
1. Rotate each vertex using the three rotation matrices
2. Apply perspective projection to get 2D coordinates
3. Draw edges between connected vertices
4. Shade edges by depth (closer = brighter)

### Interactive Demo

In the **3D Rotation** tab:
- Three sliders control rotation around X, Y, and Z axes
- Enable **Auto-rotate** for continuous animated rotation
- The wireframe cube shows depth cues through edge brightness gradients
- Vertex colors fade with distance for enhanced 3D perception

---

## 4. Eigendecomposition

### The Eigenvalue Problem

For a square matrix **A**, a vector **v** is an **eigenvector** if:

```
A·v = λ·v
```

Where `λ` (lambda) is the corresponding **eigenvalue**. In geometric terms:
- **Eigenvectors**: directions that are preserved (only stretched/compressed, not rotated)
- **Eigenvalues**: the scaling factor for each eigenvector direction

### Computing Eigenvalues

For a 2×2 matrix `A = [[a, b], [c, d]]`:

1. Compute the **characteristic polynomial**:
   ```
   det(A − λI) = λ² − tr(A)·λ + det(A) = 0
   ```
   where `tr(A) = a + d` and `det(A) = ad − bc`

2. Solve the quadratic:
   ```
   λ₁,₂ = (tr(A) ± √(tr² − 4·det)) / 2
   ```

3. The **discriminant** `Δ = tr² − 4·det` determines the nature:
   - `Δ > 0`: two distinct real eigenvalues
   - `Δ = 0`: one repeated eigenvalue
   - `Δ < 0`: complex eigenvalues (no real eigenvectors)

### Computing Eigenvectors

For each eigenvalue `λᵢ`, solve the homogeneous system:

```
(A − λᵢI)·v = 0
```

For example, with `λ₁`:
```
(a−λ₁)·x + b·y = 0
c·x + (d−λ₁)·y = 0
```

The solution gives the eigenvector direction `v₁`.

### Geometric Interpretation

- **Eigenvalues with |λ| > 1**: stretch in that direction
- **Eigenvalues with |λ| < 1**: compress in that direction
- **Eigenvalues with λ < 0**: reverse direction
- **Complex eigenvalues**: rotation (no real invariant direction)

### Diagonalization

If a matrix has two linearly independent eigenvectors, it can be **diagonalized**:

```
A = P·D·P⁻¹
```

Where:
- **P** has eigenvectors as columns
- **D** is a diagonal matrix with eigenvalues on the diagonal

This makes computing powers of A trivial: `Aⁿ = P·Dⁿ·P⁻¹`

### Interactive Demo

In the **Eigenvectors** tab:
- A set of unit vectors at evenly spaced angles is shown in gray
- The transformed versions appear in teal
- **Eigenvectors** (directions that don't change) are highlighted in red
- The output panel displays computed eigenvalues `λ₁, λ₂` and their corresponding unit eigenvectors
- Adjust the 2×2 matrix sliders to see how eigenvalues and eigenvectors change

---

## Further Reading

- Strang, G. *Introduction to Linear Algebra*. Wellesley-Cambridge Press.
- 3Blue1Brown. *Essence of Linear Algebra* (video series).
- Axler, S. *Linear Algebra Done Right*. Springer.
- Wikipedia: [Eigenvalues and eigenvectors](https://en.wikipedia.org/wiki/Eigenvalues_and_eigenvectors), [Linear map](https://en.wikipedia.org/wiki/Linear_map), [3D projection](https://en.wikipedia.org/wiki/3D_projection)
