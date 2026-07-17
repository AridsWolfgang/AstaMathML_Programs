import math

def sqrt_custom(S):
    if S <= 0:
        return 0.0
    x = S
    while True:
        nxt = (x + S/x) / 2
        if abs(nxt - x) < 1e-15:
            return nxt
        x = nxt

def run_roots_module():
    print("\n  --- Roots & Powers ---")
    print("  1) sqrt(x)")
    print("  2) cbrt(x)")
    print("  3) nth_root & hypot")
    print("  4) All")
    choice = int(input("  Choice: "))
    print()

    if choice == 1 or choice == 4:
        xs = [0, 1, 2, 4, 9, 10, 25, 100, 0.25]
        print("  >> SQRT(x) -- Newton")
        print("     x     |  our_sqrt    |  exact")
        print("     ------+-------------+-----------")
        for x in xs:
            our = sqrt_custom(x)
            exact = math.sqrt(x)
            print(f"     {x:4g}  |  {our:.10f}  |  {exact}")
        print()

    if choice == 2 or choice == 4:
        xs = [0, 1, 8, 27, 64, 100, -8, -27]
        print("  >> CBRT(x)")
        print("     x     |  our_cbrt    |  exact")
        print("     ------+-------------+-----------")
        for x in xs:
            if x == 0:
                print(f"     {x:4g}  |  0")
                continue
            neg = x < 0
            val = -x if neg else x
            v = float(val)
            while True:
                nxt = (2*v + val/(v*v)) / 3
                if abs(nxt - v) < 1e-15:
                    v = -nxt if neg else nxt
                    break
                v = nxt
            print(f"     {x:4g}  |  {v:.10f}  |  {math.cbrt(x)}")
        print()

    if choice == 3 or choice == 4:
        print("  >> HYPOT")
        print(f"     hypot(3, 4) = {sqrt_custom(3*3+4*4)}  (exact: 5)")
        print(f"     hypot(5, 12) = {sqrt_custom(5*5+12*12)}  (exact: 13)")
        print()

    input("  Press Enter...")
