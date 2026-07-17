import math

def exp_taylor(x, t=15):
    s, term = 1.0, 1.0
    for n in range(1, t+1):
        term *= x / n
        s += term
    return s

def exp_custom(x, t=15):
    if x < 0:
        return 1.0 / exp_custom(-x, t)
    if x > 1:
        h = exp_custom(x/2, t)
        return h * h
    return exp_taylor(x, t)

def run_exp_log_module():
    print("\n  --- Exponentials & Logarithms ---")
    print("  1) exp(x)")
    print("  2) ln(x)")
    print("  3) log10 & pow")
    print("  4) All")
    choice = int(input("  Choice: "))
    print()

    if choice == 1 or choice == 4:
        xs = [-2, -1, 0, 0.5, 1, 2, 3, 5]
        print("  >> EXP(x) -- 15 Taylor terms")
        print("     x    |  our_exp     |  exact       |  error")
        print("     ------+-------------+-------------+----------")
        for x in xs:
            our = exp_custom(x, 15)
            exact = math.exp(x)
            err = our - exact
            print(f"     {x:4g}  |  {our:.8f}  |  {exact:.8f}  |  {err:.6e}")
        print()

    if choice == 2 or choice == 4:
        def ln_custom(x, iters=10):
            y = x/2
            for _ in range(iters):
                ey = exp_custom(y, 15)
                y = y - (ey - x) / ey
            return y

        xs = [0.5, 1, 2, 3, 10, 100]
        print("  >> LN(x) -- Newton's method")
        print("     x    |  our_ln      |  exact       |  error")
        print("     ------+-------------+-------------+----------")
        for x in xs:
            our = ln_custom(x, 10)
            exact = math.log(x)
            err = our - exact
            print(f"     {x:4g}  |  {our:.8f}  |  {exact:.8f}  |  {err:.6e}")
        print()

    if choice == 3 or choice == 4:
        def my_ln(x):
            y = x/2
            for _ in range(10):
                ey = exp_custom(y, 15)
                y = y - (ey - x) / ey
            return y

        def my_log10(x):
            return my_ln(x) / my_ln(10)

        def my_pow(x, y):
            return exp_custom(y * my_ln(x), 15)

        print("  >> LOG10 & POW")
        print()
        print(f"     log10(100)   = {my_log10(100)}  (exact: 2)")
        print(f"     log10(1000)  = {my_log10(1000)} (exact: 3)")
        print(f"     pow(2, 10)   = {my_pow(2, 10)} (exact: 1024)")
        print(f"     pow(5, 3)    = {my_pow(5, 3)} (exact: 125)")
        print(f"     pow(2, 0.5)  = {my_pow(2, 0.5)} (exact: ~1.4142)")
        print()

    input("  Press Enter...")
