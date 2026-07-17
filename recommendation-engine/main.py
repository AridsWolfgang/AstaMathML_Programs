import sys
import os

sys.path.insert(0, os.path.dirname(__file__))

from data.ratings import USERS, ITEMS, RATINGS, build_matrix, get_user_name, get_item_name
from collaborative import CollaborativeFiltering
from matrix_factorization import FunkSVD
from metrics import rmse, mae

HEADER = """
  +--------------------------------------------+
  |       RECOMMENDATION ENGINE  v1.0          |
  |  Collaborative Filtering + FunkSVD         |
  |  Built from scratch in Python              |
  +--------------------------------------------+
"""


def show_menu():
    print("\n  --- Recommendation Engine ---")
    print("  1) View user-item rating matrix")
    print("  2) User-based CF (predict for a user)")
    print("  3) Item-based CF (predict for a user)")
    print("  4) Run FunkSVD matrix factorization")
    print("  5) Compare all methods (RMSE/MAE)")
    print("  6) Recommend top-N for a user")
    print("  Q) Quit")
    print("  Choice: ", end="")


def print_matrix(mat, mask, label="Ratings"):
    print(f"\n  {label}:")
    header = "  " + " " * 14 + "".join(f"  [{i:2d}]" for i in range(len(ITEMS)))
    print(header)
    for u in range(len(USERS)):
        row = ""
        for i in range(len(ITEMS)):
            if mask[u][i]:
                row += f"  {mat[u][i]:4.0f} " if mat[u][i] else "    0  "
            else:
                row += "     .  "
        print(f"  {USERS[u]:13s} {row}")
    print()

    for i, name in enumerate(ITEMS):
        print(f"  [{i:2d}] {name}")
    print()


def predict_for_user(mat, mask, method="user"):
    print()
    print(f"  User names: {', '.join(f'{i}:{USERS[i]}' for i in range(len(USERS)))}")
    try:
        u = int(input("  Enter user index: "))
    except ValueError:
        print("  Invalid.")
        return
    if u < 0 or u >= len(USERS):
        print("  Invalid user.")
        return

    cf = CollaborativeFiltering(mat, mask, len(USERS), len(ITEMS))
    print(f"\n  Predictions for {USERS[u]} (method={method}-based CF):")
    for i in range(len(ITEMS)):
        if not mask[u][i]:
            if method == "user":
                p = cf.user_based_cf(u, i)
            else:
                p = cf.item_based_cf(u, i)
            print(f"    {ITEMS[i]:30s} -> predicted {p:.2f}")


def run_svd(mat, mask):
    print(f"\n  Running FunkSVD (latent factors={10}, epochs={100})...")
    svd = FunkSVD(n_factors=10, lr=0.005, reg=0.02, epochs=100)
    svd.fit(mat, mask, len(USERS), len(ITEMS))
    print("  Done.")

    print(f"\n  Predicted ratings (unrated cells):")
    for u in range(len(USERS)):
        print(f"  {USERS[u]}:")
        for i in range(len(ITEMS)):
            if not mask[u][i]:
                p = svd._predict(u, i)
                print(f"    {ITEMS[i]:30s} -> predicted {p:.2f}")

    print(f"\n  Learned user factors (first 3 dims):")
    for u in range(len(USERS)):
        print(f"    {USERS[u]}: [{svd.user_factors[u][0]:+.4f}, {svd.user_factors[u][1]:+.4f}, {svd.user_factors[u][2]:+.4f}, ...]")


def compare_methods(mat, mask):
    import time

    print(f"\n  --- Method Comparison ---\n")

    print("  Training user-based CF...")
    t0 = time.time()
    cf_user = CollaborativeFiltering(mat, mask, len(USERS), len(ITEMS))
    preds_user = cf_user.predict_all(method="user")
    t_user = time.time() - t0
    rmse_user = rmse(preds_user, mat, mask)
    mae_user = mae(preds_user, mat, mask)

    print("  Training item-based CF...")
    t0 = time.time()
    cf_item = CollaborativeFiltering(mat, mask, len(USERS), len(ITEMS))
    preds_item = cf_item.predict_all(method="item")
    t_item = time.time() - t0
    rmse_item = rmse(preds_item, mat, mask)
    mae_item = mae(preds_item, mat, mask)

    print("  Training FunkSVD (100 epochs)...")
    t0 = time.time()
    svd = FunkSVD(n_factors=10, lr=0.005, reg=0.02, epochs=100)
    svd.fit(mat, mask, len(USERS), len(ITEMS))
    preds_svd = [[0.0] * len(ITEMS) for _ in range(len(USERS))]
    for u in range(len(USERS)):
        for i in range(len(ITEMS)):
            if not mask[u][i]:
                preds_svd[u][i] = svd._predict(u, i)
    t_svd = time.time() - t0
    rmse_svd = rmse(preds_svd, mat, mask)
    mae_svd = mae(preds_svd, mat, mask)

    print(f"\n  {'Method':25s} {'RMSE':>8s} {'MAE':>8s} {'Time':>8s}")
    print(f"  {'-'*25} {'-'*8} {'-'*8} {'-'*8}")
    print(f"  {'User-based CF':25s} {rmse_user:8.4f} {mae_user:8.4f} {t_user:7.2f}s")
    print(f"  {'Item-based CF':25s} {rmse_item:8.4f} {mae_item:8.4f} {t_item:7.2f}s")
    print(f"  {'FunkSVD (10 factors)':25s} {rmse_svd:8.4f} {mae_svd:8.4f} {t_svd:7.2f}s")
    print()


def recommend_for_user(mat, mask):
    print()
    print(f"  Users: {', '.join(f'{i}:{USERS[i]}' for i in range(len(USERS)))}")
    try:
        u = int(input("  Enter user index: "))
    except ValueError:
        print("  Invalid.")
        return
    if u < 0 or u >= len(USERS):
        print("  Invalid user.")
        return

    try:
        n = int(input("  How many recommendations? "))
    except ValueError:
        n = 5

    svd = FunkSVD(n_factors=10, lr=0.005, reg=0.02, epochs=100)
    svd.fit(mat, mask, len(USERS), len(ITEMS))

    predictions = [(i, svd._predict(u, i)) for i in range(len(ITEMS)) if not mask[u][i]]
    predictions.sort(key=lambda x: -x[1])
    top = predictions[:n]

    print(f"\n  Top {n} recommendations for {USERS[u]}:")
    for i, p in top:
        print(f"    {ITEMS[i]:30s} (predicted: {p:.2f})")

    actually_rated = [(i, mat[u][i]) for i in range(len(ITEMS)) if mask[u][i]]
    actually_rated.sort(key=lambda x: -x[1])
    print(f"\n  {USERS[u]}'s actual top-rated:")
    for i, r in actually_rated[:n]:
        print(f"    {ITEMS[i]:30s} (actual: {r:.0f})")


def main():
    print(HEADER)
    mat, mask = build_matrix()

    svd = None

    while True:
        show_menu()
        choice = input().strip().lower()

        if choice == "1":
            print_matrix(mat, mask)
        elif choice == "2":
            predict_for_user(mat, mask, method="user")
        elif choice == "3":
            predict_for_user(mat, mask, method="item")
        elif choice == "4":
            run_svd(mat, mask)
        elif choice == "5":
            compare_methods(mat, mask)
        elif choice == "6":
            recommend_for_user(mat, mask)
        elif choice == "q":
            print("\n  Goodbye!")
            break
        else:
            print("  Invalid choice.")


if __name__ == "__main__":
    main()
