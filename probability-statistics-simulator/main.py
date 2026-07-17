import math
import random
import os
import sys

from distributions import Normal, Binomial, Poisson, Uniform, ascii_histogram
from sampling import monte_carlo_pi, monte_carlo_integral, dice_roll_simulation, bootstrap_mean_ci
from stats import mean, variance, std, skewness, kurtosis
from tests import t_test_independent, chi_square_test, one_sample_t_test
from data.datasets import (
    normal_population,
    binomial_population,
    poisson_population,
    uniform_population,
    custom_mixture,
)


def clear():
    os.system("cls" if os.name == "nt" else "clear")


def pause():
    input("\nPress Enter to continue...")


def menu_distribution_explorer():
    clear()
    print("=" * 60)
    print("              DISTRIBUTION EXPLORER")
    print("=" * 60)
    print("1. Normal distribution")
    print("2. Binomial distribution")
    print("3. Poisson distribution")
    print("4. Uniform distribution")
    print("5. Back")
    choice = input("\nSelect distribution: ").strip()

    if choice == "1":
        mu = float(input("Mean (default 0): ") or 0)
        sigma = float(input("Std dev (default 1): ") or 1)
        n = int(input("Sample size (default 5000): ") or 5000)
        dist = Normal(mu, sigma)
        data = dist.sample(n)
        print(ascii_histogram(data, title=f"Normal({mu}, {sigma})"))
    elif choice == "2":
        n_trials = int(input("n trials (default 20): ") or 20)
        p = float(input("p success (default 0.5): ") or 0.5)
        n_samples = int(input("Sample size (default 5000): ") or 5000)
        dist = Binomial(n_trials, p)
        data = dist.sample(n_samples)
        print(ascii_histogram(data, title=f"Binomial({n_trials}, {p})"))
    elif choice == "3":
        lam = float(input("Lambda (default 4): ") or 4)
        n = int(input("Sample size (default 5000): ") or 5000)
        dist = Poisson(lam)
        data = dist.sample(n)
        print(ascii_histogram(data, title=f"Poisson({lam})"))
    elif choice == "4":
        a = float(input("a (default 0): ") or 0)
        b = float(input("b (default 10): ") or 10)
        n = int(input("Sample size (default 5000): ") or 5000)
        dist = Uniform(a, b)
        data = dist.sample(n)
        print(ascii_histogram(data, title=f"Uniform({a}, {b})"))
    else:
        return

    if data:
        print(f"\nMean: {mean(data):.4f}")
        print(f"Variance: {variance(data):.4f}")
        print(f"Skewness: {skewness(data):.4f}")
        print(f"Kurtosis (excess): {kurtosis(data):.4f}")
    pause()


def menu_monte_carlo():
    clear()
    print("=" * 60)
    print("             MONTE CARLO SIMULATION")
    print("=" * 60)
    print("1. Estimate Pi")
    print("2. Dice roll distribution")
    print("3. Monte Carlo integration")
    print("4. Bootstrap confidence interval (mean)")
    print("5. Back")
    choice = input("\nSelect option: ").strip()

    if choice == "1":
        n = int(input("Number of samples (default 100000): ") or 100000)
        pi_est = monte_carlo_pi(n)
        print(f"\nEstimated Pi: {pi_est:.6f}")
        print(f"Actual Pi:    {math.pi:.6f}")
        print(f"Error:        {abs(pi_est - math.pi):.6f}")
    elif choice == "2":
        dice = int(input("Number of dice (default 2): ") or 2)
        sides = int(input("Sides per die (default 6): ") or 6)
        rolls = int(input("Number of rolls (default 100000): ") or 100000)
        probs = dice_roll_simulation(rolls, dice, sides)
        max_prob = max(probs.values()) if probs else 1
        bar_width = 50
        print(f"\nDice roll distribution ({dice}d{sides}, {rolls} rolls):")
        for total, p in probs.items():
            bar = "#" * int(p / max_prob * bar_width)
            print(f"  {total:2d}: {bar} {p*100:.2f}%")
    elif choice == "3":
        print("\nIntegrate f(x) = x^2 over [0, 1] (true value = 1/3)")
        print("Integrate f(x) = sin(x) over [0, pi] (true value = 2)")
        expr = input("Choose function (x^2 or sin): ").strip()
        a = float(input("Lower bound (default 0): ") or 0)
        b = float(input("Upper bound (default 1): ") or 1)
        n = int(input("Samples (default 100000): ") or 100000)
        if expr == "sin":
            result = monte_carlo_integral(math.sin, a, b, n)
            true_val = -math.cos(b) + math.cos(a)
        else:
            result = monte_carlo_integral(lambda x: x * x, a, b, n)
            true_val = (b**3 - a**3) / 3
        print(f"\nEstimated: {result:.6f}")
        print(f"True:      {true_val:.6f}")
        print(f"Error:     {abs(result - true_val):.6f}")
    elif choice == "4":
        dist_choice = input("Generate data from (normal/binomial): ").strip().lower()
        n = int(input("Sample size (default 1000): ") or 1000)
        if "bin" in dist_choice:
            data = binomial_population(n=20, p=0.5, size=n)
        else:
            data = normal_population(mean=0, std=1, size=n)
        resamples = int(input("Number of bootstrap resamples (default 1000): ") or 1000)
        lo, hi = bootstrap_mean_ci(data, resamples)
        print(f"\nSample mean: {mean(data):.4f}")
        print(f"95% CI for mean: [{lo:.4f}, {hi:.4f}]")
    pause()


def menu_clt_demo():
    clear()
    print("=" * 60)
    print("         CENTRAL LIMIT THEOREM DEMO")
    print("=" * 60)
    print("The CLT states that the sampling distribution of the sample")
    print("mean approaches a normal distribution as sample size increases,")
    print("regardless of the population distribution.")
    print()
    print("Select population distribution:")
    print("1. Uniform(0, 10)")
    print("2. Exponential (skewed)")
    print("3. Binomial(20, 0.1) (skewed)")
    choice = input("\nChoice: ").strip()

    n_samples = 10000

    if choice == "1":
        population = uniform_population(0, 10, n_samples)
        pop_label = "Uniform(0, 10)"
    elif choice == "2":
        population = [(-1 / 0.5) * math.log(1 - random.random()) for _ in range(n_samples)]
        pop_label = "Exponential(rate=0.5)"
    elif choice == "3":
        population = binomial_population(20, 0.1, n_samples)
        pop_label = "Binomial(20, 0.1)"
    else:
        population = uniform_population(0, 10, n_samples)
        pop_label = "Uniform(0, 10)"

    sample_sizes = [2, 5, 30]
    print(f"\nPopulation: {pop_label}")
    print(f"Population mean: {mean(population):.4f}")
    print(f"Population variance: {variance(population):.4f}")
    print()

    for size in sample_sizes:
        means_of_means = []
        for _ in range(5000):
            sample = random.choices(population, k=size)
            means_of_means.append(mean(sample))
        print(f"--- Sample size n={size} ---")
        print(ascii_histogram(means_of_means, width=50, title=f"Means (n={size})"))
        print(f"  Mean of means: {mean(means_of_means):.4f}")
        print(f"  Std of means:  {std(means_of_means):.4f}")
        print(f"  Skewness:      {skewness(means_of_means):.4f}")
        print(f"  Kurtosis:      {kurtosis(means_of_means):.4f}")
        print()

    pause()


def menu_hypothesis_testing():
    clear()
    print("=" * 60)
    print("           HYPOTHESIS TESTING DEMO")
    print("=" * 60)
    print("1. One-sample t-test")
    print("2. Two-sample independent t-test")
    print("3. Chi-square goodness-of-fit test")
    print("4. Back")
    choice = input("\nSelect test: ").strip()

    if choice == "1":
        dist = input("Generate data from normal (y/n): ").strip().lower()
        n = int(input("Sample size (default 50): ") or 50)
        if dist == "y":
            mu = float(input("True mean (default 0): ") or 0)
            sigma = float(input("Std dev (default 1): ") or 1)
            data = Normal(mu, sigma).sample(n)
        else:
            data = [random.gauss(0.5, 1) for _ in range(n)]
        mu0 = float(input("Null hypothesis mean (default 0): ") or 0)
        t_stat, p_val = one_sample_t_test(data, mu0)
        print(f"\nSample mean: {mean(data):.4f}")
        print(f"Sample std:  {std(data, ddof=1):.4f}")
        print(f"t-statistic: {t_stat:.4f}")
        print(f"p-value:     {p_val:.4f}")
        if p_val is not None and p_val < 0.05:
            print("Result: REJECT H0 (p < 0.05)")
        else:
            print("Result: Fail to reject H0 (p >= 0.05)")
    elif choice == "2":
        print("\nGenerating two samples...")
        n1 = int(input("Sample 1 size (default 30): ") or 30)
        n2 = int(input("Sample 2 size (default 30): ") or 30)
        mu1 = float(input("Sample 1 mean (default 0): ") or 0)
        mu2 = float(input("Sample 2 mean (default 1): ") or 1)
        sigma = float(input("Std dev (default 1): ") or 1)
        s1 = Normal(mu1, sigma).sample(n1)
        s2 = Normal(mu2, sigma).sample(n2)
        t_stat, p_val = t_test_independent(s1, s2, equal_var=True)
        print(f"\nSample 1 mean: {mean(s1):.4f}")
        print(f"Sample 2 mean: {mean(s2):.4f}")
        print(f"Difference:    {mean(s1) - mean(s2):.4f}")
        print(f"t-statistic:   {t_stat:.4f}")
        print(f"p-value:       {p_val:.4f}")
        if p_val is not None and p_val < 0.05:
            print("Result: REJECT H0 (p < 0.05) — means differ significantly")
        else:
            print("Result: Fail to reject H0 (p >= 0.05)")
    elif choice == "3":
        print("\nChi-square test for goodness of fit")
        print("Testing if observed die rolls follow uniform distribution")
        sides = int(input("Number of sides (default 6): ") or 6)
        rolls = int(input("Number of rolls (default 6000): ") or 6000)
        observed = [0] * sides
        for _ in range(rolls):
            observed[random.randint(0, sides - 1)] += 1
        chi2, p_val = chi_square_test(observed)
        print(f"\nObserved frequencies: {observed}")
        expected = [rolls / sides] * sides
        print(f"Expected frequencies: {[int(e) for e in expected]}")
        print(f"Chi-square:           {chi2:.4f}")
        print(f"p-value:              {p_val:.4f}")
        if p_val is not None and p_val < 0.05:
            print("Result: REJECT H0 — not uniform (p < 0.05)")
        else:
            print("Result: Fail to reject H0 — uniform (p >= 0.05)")
    pause()


def main():
    while True:
        clear()
        print("=" * 60)
        print("     PROBABILITY & STATISTICS SIMULATOR")
        print("=" * 60)
        print("1. Distribution Explorer")
        print("2. Monte Carlo Simulation")
        print("3. Central Limit Theorem Demo")
        print("4. Hypothesis Testing Demo")
        print("5. Exit")
        choice = input("\nSelect option: ").strip()

        if choice == "1":
            menu_distribution_explorer()
        elif choice == "2":
            menu_monte_carlo()
        elif choice == "3":
            menu_clt_demo()
        elif choice == "4":
            menu_hypothesis_testing()
        elif choice == "5":
            print("\nGoodbye!")
            break


if __name__ == "__main__":
    main()
