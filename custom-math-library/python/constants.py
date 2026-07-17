import math

def run_constants_module():
    print("\n  --- Constants ---")
    print("  1) Pi (slow: Leibniz)")
    print("  2) Pi (fast: Machin)")
    print("  3) e (Taylor)")
    print("  4) Phi (continued fraction)")
    print("  5) All")
    choice = int(input("  Choice: "))
    print()

    if choice == 1 or choice == 5:
        print("  >> PI (Leibniz series)")
        print("     pi = 4 x (1 - 1/3 + 1/5 - 1/7 + ...)\n")
        for t in (10, 100, 1000, 10000, 100000):
            s = sum((1.0 / (2*n+1)) * (1 if n%2==0 else -1) for n in range(t))
            pi = 4*s
            err = pi - math.pi
            print(f"     {t:6d} terms: {pi:.12f}  error: {err:.6e}")
        print("     Lesson: Leibniz converges VERY slowly.\n")

    if choice == 2 or choice == 5:
        def arctan(x, n):
            s, xp = 0.0, x
            for i in range(n):
                s += (xp / (2*i+1)) * (1 if i%2==0 else -1)
                xp *= x*x
            return s

        print("  >> PI (Machin's formula)")
        print("     pi/4 = 4*arctan(1/5) - arctan(1/239)\n")
        for t in range(2, 13, 2):
            pi = 4*(4*arctan(1/5, t) - arctan(1/239, t))
            err = pi - math.pi
            print(f"     {t} terms: {pi:.12f}  error: {err:.6e}")
        print("     Lesson: 10 terms ~ double precision!\n")

    if choice == 3 or choice == 5:
        print("  >> EULER's e")
        print("     e = 1 + 1/1! + 1/2! + 1/3! + ...\n")
        for t in range(2, 21, 2):
            s, fact = 1.0, 1
            for n in range(1, t+1):
                fact *= n
                s += 1.0 / fact
            err = s - math.e
            print(f"     {t:2d} terms: {s:.12f}  error: {err:.6e}")
        print("     Lesson: ~15 terms = full precision.\n")

    if choice == 4 or choice == 5:
        print("  >> PHI (continued fraction)")
        print("     phi = 1 + 1/(1 + 1/(1 + ...))\n")
        exact = (1 + math.sqrt(5)) / 2
        for t in range(1, 11):
            phi = 1.0
            for _ in range(t):
                phi = 1 + 1/phi
            err = phi - exact
            print(f"     {t} iters: {phi:.12f}  error: {err:.6e}")
        print("     Lesson: Continued fractions converge fast.\n")

    input("  Press Enter...")
