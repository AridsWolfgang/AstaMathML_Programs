import math

def sin_taylor(x, t=10):
    while x > math.pi:
        x -= 2*math.pi
    while x < -math.pi:
        x += 2*math.pi
    sign = 1
    if x > math.pi/2:
        x = math.pi - x
    elif x < -math.pi/2:
        x = -math.pi - x
        sign = -1
    s, term, x2 = 0.0, x, x*x
    for n in range(t):
        if n > 0:
            term *= x2 / (2*n * (2*n+1))
        s += term if n % 2 == 0 else -term
    return sign * s

def cos_taylor(x, t=10):
    return sin_taylor(x + math.pi/2, t)

def run_trig_module():
    print("\n  --- Trigonometry ---")
    print("  1) sin(x)")
    print("  2) cos(x)")
    print("  3) tan(x)")
    print("  4) All")
    choice = int(input("  Choice: "))
    print()

    angles = [0, 30, 45, 60, 90, 180, 360]
    labels = ["0deg", "30deg", "45deg", "60deg", "90deg", "180deg", "360deg"]

    if choice == 1 or choice == 4:
        print("  >> SIN(x) -- Taylor series (10 terms)")
        print("     Angle  |  our_sin     |  exact       |  error")
        print("     -------+-------------+-------------+----------")
        for i, a in enumerate(angles):
            rad = a * math.pi / 180
            our = sin_taylor(rad, 10)
            exact = math.sin(rad)
            err = our - exact
            print(f"     {labels[i]:>4s}  |  {our:.8f}  |  {exact:.8f}  |  {err:.6e}")
        print()

    if choice == 2 or choice == 4:
        print("  >> COS(x)")
        print("     Angle  |  our_cos     |  exact       |  error")
        print("     -------+-------------+-------------+----------")
        for i, a in enumerate(angles):
            rad = a * math.pi / 180
            our = cos_taylor(rad, 10)
            exact = math.cos(rad)
            err = our - exact
            print(f"     {labels[i]:>4s}  |  {our:.8f}  |  {exact:.8f}  |  {err:.6e}")
        print()

    if choice == 3 or choice == 4:
        print("  >> TAN(x) = sin(x)/cos(x)")
        print("     Angle  |  our_tan     |  exact       |  error")
        print("     -------+-------------+-------------+----------")
        for i, a in enumerate(angles):
            if a == 90:
                print("     90deg  |  undefined")
                continue
            rad = a * math.pi / 180
            our = sin_taylor(rad, 10) / cos_taylor(rad, 10)
            exact = math.tan(rad)
            err = our - exact
            print(f"     {labels[i]:>4s}  |  {our:.8f}  |  {exact:.8f}  |  {err:.6e}")
        print()

    input("  Press Enter...")
