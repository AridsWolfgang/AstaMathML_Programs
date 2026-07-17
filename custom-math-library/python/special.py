import math

def run_special_module():
    print("\n  --- Special Functions ---")
    print("  1) Factorial")
    print("  2) Fibonacci")
    print("  3) GCD & Primes")
    print("  4) ML Activations")
    print("  5) All")
    choice = int(input("  Choice: "))
    print()

    if choice == 1 or choice == 5:
        print("  >> FACTORIAL")
        f = 1
        for i in range(0, 11):
            if i > 1:
                f *= i
            print(f"     {i}! = {f}")
        print()

    if choice == 2 or choice == 5:
        print("  >> FIBONACCI")
        fib = [0, 1]
        for i in range(2, 21):
            fib.append(fib[i-1] + fib[i-2])
        print("    ", " ".join(str(x) for x in fib))
        print()

    if choice == 3 or choice == 5:
        def gcd(a, b):
            while b:
                a, b = b, a % b
            return a
        print(f"  gcd(48, 18) = {gcd(48, 18)}")
        print(f"  gcd(1071, 462) = {gcd(1071, 462)}")
        print()
        print("  Primes up to 50:", end=" ")
        for n in range(2, 51):
            prime = all(n % d != 0 for d in range(2, int(math.isqrt(n)) + 1))
            if prime:
                print(n, end=" ")
        print("\n")

    if choice == 4 or choice == 5:
        def exp_c(x):
            s, t = 1.0, 1.0
            for n in range(1, 16):
                t *= x / n
                s += t
            return s

        print("  >> SIGMOID & RELU")
        print("     x   |  sigmoid    |  ReLU")
        print("     -----+------------+-------")
        for x in [-5.0, -2.0, -1.0, 0.0, 1.0, 2.0, 5.0]:
            if x > 0:
                sig = 1 / (1 + exp_c(-x))
            else:
                sig = exp_c(x) / (1 + exp_c(x))
            rel = x if x > 0 else 0
            print(f"     {x:3g}  |  {sig:.6f}  |  {rel}")
        print()

    input("  Press Enter...")
