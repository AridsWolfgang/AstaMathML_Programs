import math
import statistics

def run_stats_module():
    print("\n  --- Statistics ---")
    print("  1) Mean & Median")
    print("  2) Variance & StdDev")
    print("  3) Correlation")
    print("  4) All")
    choice = int(input("  Choice: "))
    print()

    scores = [85, 92, 78, 90, 88, 76, 95, 89, 84, 91, 73, 87]

    if choice == 1 or choice == 4:
        mu = sum(scores) / len(scores)
        s = sorted(scores)
        n = len(s)
        med = s[n//2] if n % 2 else (s[n//2-1] + s[n//2]) / 2
        print(f"  Mean   = {mu}")
        print(f"  Median = {med}")
        print()

    if choice == 2 or choice == 4:
        mu = sum(scores) / len(scores)
        var = sum((x-mu)**2 for x in scores) / len(scores)
        sd = math.sqrt(var)
        print(f"  Variance = {var}")
        print(f"  StdDev   = {sd}")
        print(f"  68% in [{mu-sd}, {mu+sd}]")
        print()

    if choice == 3 or choice == 4:
        hours = [2,3,5,4,6,1,7,5,3,6,2,4]
        scr =  [72,78,88,85,92,65,95,90,80,91,70,82]
        n = len(hours)
        mx = sum(hours)/n
        my = sum(scr)/n
        cov = sum((hours[i]-mx)*(scr[i]-my) for i in range(n))
        vx = sum((hours[i]-mx)**2 for i in range(n))
        vy = sum((scr[i]-my)**2 for i in range(n))
        r = cov / math.sqrt(vx*vy)
        print(f"  Pearson's r = {r} (strong correlation)")
        print()

    input("  Press Enter...")
