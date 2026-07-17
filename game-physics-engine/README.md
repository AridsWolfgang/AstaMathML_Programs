# Game Physics Engine

An interactive C++17 command-line toolkit for 2D physics simulation. Implements projectile motion (with drag), spring-mass systems, collision detection/response, and pendulum dynamics.

## Building

```bash
cd game-physics-engine/src
g++ -std=c++17 -Wall -Wextra -O2 main.cpp physics.cpp projectile.cpp spring.cpp collision.cpp pendulum.cpp -o ../build/physics.exe
```

Run: `build\physics.exe`

## Modules

### Vec2 and RigidBody (`physics.hpp`)

Foundational 2D vector class with basic arithmetic, dot product, length, and normalization. `RigidBody` holds position, velocity, and mass.

**Constants**: `G = 9.81 m/s²`, `π`

### 1. Projectile Motion (`projectile.cpp`)

Simulates a projectile launched at angle `θ` with initial speed `v₀`, subject to gravity and quadratic air drag.

**Physics**:
- Horizontal: `F_drag,x = -½ ρ C_d A v v_x` (simplified to `c * v²` in direction opposite to velocity).
- Vertical: `F_drag,y = -mg - c * v²` (drag + gravity).
- Integration: Semi-implicit Euler (update velocity first, then position).

**Key parameters**: initial velocity, launch angle, drag coefficient.

### 2. Spring-Mass System (`spring.cpp`)

Simulates a mass attached to a spring with Hooke's law and linear damping.

**Physics**:
- Restoring force: `F = -k x` (Hooke's law)
- Damping force: `F_damp = -c v`
- Equation of motion: `m x'' + c x' + k x = 0`

**Integration**: Explicit Euler.

**Behavior** (depending on damping ratio `ζ = c / (2√(mk))`):
- `ζ < 1`: Underdamped (oscillates with decaying amplitude).
- `ζ = 1`: Critically damped (fastest return to equilibrium).
- `ζ > 1`: Overdamped (slow return, no oscillation).

### 3. Collision Detection (`collision.cpp`)

Detects and resolves overlaps between circles and AABBs (Axis-Aligned Bounding Boxes).

**Shape types**:
- **AABB**: Defined by `min` and `max` corners. Overlap test uses separating axis theorem (SAT) for 2D axes.
- **Circle**: Defined by center and radius. Overlap test compares center distance to sum of radii.

**Resolution**:
- **Circle-Circle**: Separates overlapping circles proportionally to inverse mass.
- **Circle-AABB**: Pushes circle to closest point on AABB surface.

### 4. Simple Pendulum (`pendulum.cpp`)

Simulates a mass on a massless rod under gravity using fourth-order Runge-Kutta (RK4) integration.

**Physics**:
- Angular acceleration: `θ'' = -(g/L) sin(θ)`
- State vector: `[θ, ω]` where `ω = θ'`
- Derivative function: `d/dt [θ, ω] = [ω, -(g/L) sin(θ)]`

**RK4 Integration**:
```
k₁ = f(tₙ, yₙ)
k₂ = f(tₙ + h/2, yₙ + h·k₁/2)
k₃ = f(tₙ + h/2, yₙ + h·k₂/2)
k₄ = f(tₙ + h, yₙ + h·k₃)
yₙ₊₁ = yₙ + (h/6)(k₁ + 2k₂ + 2k₃ + k₄)
```

**Small-angle approximation**: For small `θ`, `sin(θ) ≈ θ`, giving period `T = 2π√(L/g)`. RK4 handles the full nonlinear equation.

## File Structure

```
game-physics-engine/
├── README.md
├── src/
│   ├── main.cpp            # Menu-driven interactive CLI
│   ├── physics.hpp         # Vec2, RigidBody, physics constants
│   ├── physics.cpp
│   ├── projectile.hpp      # Projectile simulation with drag
│   ├── projectile.cpp
│   ├── spring.hpp          # Spring-mass (Hooke + damping)
│   ├── spring.cpp
│   ├── collision.hpp       # AABB, Circle collision + resolution
│   ├── collision.cpp
│   ├── pendulum.hpp        # Simple pendulum with RK4
│   └── pendulum.cpp
└── build/
    └── physics.exe
```

## References

- Baraff & Witkin, *Physically Based Modeling: Principles and Practice* (SIGGRAPH 2001)
- Hecker, *Physics for Game Developers* (GDC 1996)
- Bourg, *Physics for Game Developers*, 2nd ed.
