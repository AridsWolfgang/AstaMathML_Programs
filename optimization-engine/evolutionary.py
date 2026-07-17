"""Genetic Algorithm from scratch."""

import math
import random


def genetic_algorithm(
    fitness_func,
    bounds,
    pop_size=100,
    generations=200,
    mutation_rate=0.1,
    crossover_rate=0.8,
    elite_size=2,
    minimize=True,
    verbose=False,
):
    """Genetic Algorithm for continuous optimization.

    Args:
        fitness_func: function taking a list of floats, returns fitness.
        bounds: list of (lo, hi) for each dimension.
        pop_size: population size.
        generations: max iterations.
        mutation_rate: probability of mutation per gene.
        crossover_rate: probability of crossover.
        elite_size: number of best individuals preserved each generation.
        minimize: if True, lower fitness is better.
        verbose: print progress.

    Returns:
        (best_solution, best_fitness, history)
    """
    n_dims = len(bounds)

    # Initialize population
    pop = []
    for _ in range(pop_size):
        ind = [lo + (hi - lo) * random.random() for lo, hi in bounds]
        pop.append(ind)

    best_ever = None
    best_fit_ever = float("inf") if minimize else -float("inf")
    history = []

    for gen in range(generations):
        # Evaluate fitness
        fits = [fitness_func(ind) for ind in pop]

        # Track best
        for ind, fit in zip(pop, fits):
            if minimize:
                if fit < best_fit_ever:
                    best_fit_ever = fit
                    best_ever = list(ind)
            else:
                if fit > best_fit_ever:
                    best_fit_ever = fit
                    best_ever = list(ind)

        history.append(best_fit_ever)
        if verbose and gen % 20 == 0:
            print(f"  gen {gen:4d}: best f = {best_fit_ever:.6e}")

        # Selection probabilities (rank-based)
        ranked = sorted(zip(fits, pop), key=lambda x: x[0], reverse=not minimize)
        ranks = list(range(len(ranked), 0, -1))
        total_rank = sum(ranks)
        probs = [r / total_rank for r in ranks]

        # Create next generation
        new_pop = []

        # Elitism
        for i in range(min(elite_size, len(ranked))):
            new_pop.append(list(ranked[i][1]))

        # Fill rest with crossover and mutation
        while len(new_pop) < pop_size:
            # Tournament selection (simpler)
            parent1 = _tournament_select(pop, fits, 3, minimize)
            parent2 = _tournament_select(pop, fits, 3, minimize)

            # Crossover
            if random.random() < crossover_rate:
                child1, child2 = _sbx_crossover(parent1, parent2, bounds)
            else:
                child1, child2 = list(parent1), list(parent2)

            # Mutation
            child1 = _gaussian_mutate(child1, mutation_rate, bounds)
            child2 = _gaussian_mutate(child2, mutation_rate, bounds)

            new_pop.append(child1)
            if len(new_pop) < pop_size:
                new_pop.append(child2)

        pop = new_pop[:pop_size]

    return best_ever, best_fit_ever, history


def _tournament_select(pop, fits, k=3, minimize=True):
    """Tournament selection: pick k random individuals, return the best."""
    idxs = random.sample(range(len(pop)), k)
    if minimize:
        best_idx = min(idxs, key=lambda i: fits[i])
    else:
        best_idx = max(idxs, key=lambda i: fits[i])
    return list(pop[best_idx])


def _sbx_crossover(p1, p2, bounds, eta=15):
    """Simulated Binary Crossover for real-coded GA."""
    n = len(p1)
    c1, c2 = [0.0] * n, [0.0] * n
    for i in range(n):
        if random.random() < 0.5:
            if abs(p1[i] - p2[i]) > 1e-10:
                u = random.random()
                if u <= 0.5:
                    beta = (2 * u) ** (1.0 / (eta + 1))
                else:
                    beta = (1.0 / (2 * (1 - u))) ** (1.0 / (eta + 1))
                c1[i] = 0.5 * ((p1[i] + p2[i]) - beta * (p2[i] - p1[i]))
                c2[i] = 0.5 * ((p1[i] + p2[i]) + beta * (p2[i] - p1[i]))
            else:
                c1[i], c2[i] = p1[i], p2[i]
        else:
            c1[i], c2[i] = p1[i], p2[i]

        # Clip to bounds
        lo, hi = bounds[i]
        c1[i] = max(lo, min(hi, c1[i]))
        c2[i] = max(lo, min(hi, c2[i]))
    return c1, c2


def _gaussian_mutate(individual, rate, bounds):
    """Gaussian mutation: add N(0, sigma) noise with given probability."""
    n = len(individual)
    child = list(individual)
    for i in range(n):
        if random.random() < rate:
            sigma = (bounds[i][1] - bounds[i][0]) * 0.1
            child[i] += random.gauss(0, sigma)
            child[i] = max(bounds[i][0], min(bounds[i][1], child[i]))
    return child
