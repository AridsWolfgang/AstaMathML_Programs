from constants import run_constants_module
from trig import run_trig_module
from exp_log import run_exp_log_module
from roots import run_roots_module
from linalg import run_linalg_module
from stats import run_stats_module
from special import run_special_module

def main():
    while True:
        print("\n" + "="*60)
        print("  CUSTOM MATH LIBRARY -- Python Edition")
        print("="*60)
        print("  1) Constants (pi, e, phi)")
        print("  2) Trigonometry (sin, cos, tan)")
        print("  3) Exponentials & Logarithms")
        print("  4) Roots & Powers")
        print("  5) Linear Algebra")
        print("  6) Statistics")
        print("  7) Special Functions")
        print("  0) Exit")
        choice = input("  Choice: ")

        if choice == "1":
            run_constants_module()
        elif choice == "2":
            run_trig_module()
        elif choice == "3":
            run_exp_log_module()
        elif choice == "4":
            run_roots_module()
        elif choice == "5":
            run_linalg_module()
        elif choice == "6":
            run_stats_module()
        elif choice == "7":
            run_special_module()
        elif choice == "0":
            print("\n  Goodbye!")
            break
        else:
            print("  Invalid choice.")

if __name__ == "__main__":
    main()
