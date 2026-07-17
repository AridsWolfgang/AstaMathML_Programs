import math

def run_linalg_module():
    print("\n  --- Linear Algebra ---")
    print("  1) Vectors")
    print("  2) Matrices")
    print("  3) 3D rotation")
    print("  4) All")
    choice = int(input("  Choice: "))
    print()

    if choice == 1 or choice == 4:
        a = [3, 4, 0]
        b = [1, 2, 3]
        print(f"  a = {a}")
        print(f"  b = {b}")
        ab = [a[i] + b[i] for i in range(3)]
        print(f"  a+b = {ab}")
        dot = sum(a[i] * b[i] for i in range(3))
        print(f"  a.b = {dot}")
        mag = math.sqrt(a[0]**2 + a[1]**2 + a[2]**2)
        print(f"  |a| = {mag}")
        print()

    if choice == 2 or choice == 4:
        I = [[1,0,0], [0,1,0], [0,0,1]]
        print("  Identity matrix:")
        for row in I:
            print(f"    {row}")
        print()

    if choice == 3 or choice == 4:
        pt = [1, 0, 0]
        c, s = math.cos(math.pi/4), math.sin(math.pi/4)
        R = [[c, -s, 0], [s, c, 0], [0, 0, 1]]
        print(f"  Point = {pt}")
        res = [sum(R[r][c] * pt[c] for c in range(3)) for r in range(3)]
        print(f"  Rotated 45deg = {res}")
        print()

    input("  Press Enter...")
