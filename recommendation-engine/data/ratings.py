USERS = ["Alice", "Bob", "Charlie", "Diana", "Eve", "Frank", "Grace", "Hank"]
ITEMS = [
    "The Matrix",         # 0
    "Inception",          # 1
    "Interstellar",       # 2
    "Pulp Fiction",       # 3
    "The Godfather",      # 4
    "Toy Story",          # 5
    "Finding Nemo",       # 6
    "The Dark Knight",    # 7
    "Frozen",             # 8
    "The Avengers",       # 9
    "The Shawshank Redemption",  # 10
    "Forrest Gump",       # 11
]

GENRES = {
    "The Matrix"      : ["Sci-Fi", "Action"],
    "Inception"       : ["Sci-Fi", "Thriller"],
    "Interstellar"    : ["Sci-Fi", "Drama"],
    "Pulp Fiction"    : ["Crime", "Drama"],
    "The Godfather"   : ["Crime", "Drama"],
    "Toy Story"       : ["Animation", "Comedy"],
    "Finding Nemo"    : ["Animation", "Adventure"],
    "The Dark Knight" : ["Action", "Crime"],
    "Frozen"          : ["Animation", "Musical"],
    "The Avengers"    : ["Action", "Sci-Fi"],
    "The Shawshank Redemption" : ["Drama"],
    "Forrest Gump"    : ["Drama", "Romance"],
}

ITEM_FEATURES = {
    "The Matrix"      : {"action": 5, "scifi": 5, "mind_bend": 4, "dark": 4, "classic": 3},
    "Inception"       : {"scifi": 4, "mind_bend": 5, "thriller": 4, "action": 3, "dream": 5},
    "Interstellar"    : {"scifi": 5, "drama": 4, "emotional": 5, "space": 5, "adventure": 4},
    "Pulp Fiction"    : {"crime": 5, "drama": 4, "dark": 4, "cult": 5, "dialogue": 5},
    "The Godfather"   : {"crime": 5, "drama": 5, "classic": 5, "mafia": 5, "epic": 5},
    "Toy Story"       : {"animation": 5, "comedy": 4, "family": 5, "heartwarming": 5, "friendship": 5},
    "Finding Nemo"    : {"animation": 5, "adventure": 5, "family": 5, "ocean": 4, "heartwarming": 4},
    "The Dark Knight" : {"action": 5, "crime": 4, "dark": 5, "comic": 4, "hero": 5},
    "Frozen"          : {"animation": 5, "musical": 5, "family": 4, "sisterhood": 5, "magic": 4},
    "The Avengers"    : {"action": 5, "scifi": 3, "comic": 5, "hero": 5, "blockbuster": 5},
    "The Shawshank Redemption" : {"drama": 5, "classic": 5, "prison": 5, "emotional": 5, "hope": 5},
    "Forrest Gump"    : {"drama": 5, "romance": 3, "comedy": 3, "historical": 4, "heartwarming": 5},
}

RATINGS = [
    # user_idx, item_idx, rating
    (0, 0, 5), (0, 1, 4), (0, 2, 5), (0, 3, 3),              # Alice
    (0, 7, 4), (0, 8, 2), (0, 10, 5), (0, 11, 4),
    (1, 0, 4), (1, 1, 5), (1, 2, 4), (1, 4, 5),              # Bob
    (1, 7, 5), (1, 9, 4), (1, 10, 4), (1, 3, 4),
    (2, 0, 3), (2, 1, 3), (2, 5, 5), (2, 6, 5),              # Charlie
    (2, 8, 5), (2, 11, 4), (2, 2, 2), (2, 4, 1),
    (3, 0, 2), (3, 3, 5), (3, 4, 5), (3, 7, 3),              # Diana
    (3, 10, 4), (3, 11, 5), (3, 1, 2), (3, 5, 1),
    (4, 0, 5), (4, 1, 5), (4, 3, 4), (4, 7, 5),              # Eve
    (4, 9, 5), (4, 10, 3), (4, 11, 3), (4, 2, 4),
    (5, 5, 5), (5, 6, 4), (5, 8, 5), (5, 11, 4),             # Frank
    (5, 4, 2), (5, 3, 2), (5, 0, 1), (5, 9, 3),
    (6, 0, 4), (6, 2, 5), (6, 5, 4), (6, 6, 5),              # Grace
    (6, 8, 5), (6, 11, 4), (6, 7, 3), (6, 1, 3),
    (7, 0, 2), (7, 1, 2), (7, 3, 4), (7, 4, 5),              # Hank
    (7, 7, 5), (7, 9, 4), (7, 10, 5), (7, 11, 3),
]


def build_matrix():
    n_users = len(USERS)
    n_items = len(ITEMS)
    mat = [[0.0] * n_items for _ in range(n_users)]
    mask = [[False] * n_items for _ in range(n_users)]
    for u, i, r in RATINGS:
        mat[u][i] = float(r)
        mask[u][i] = True
    return mat, mask


def get_user_name(idx):
    return USERS[idx]


def get_item_name(idx):
    return ITEMS[idx]


def get_item_features():
    import math
    items = list(ITEM_FEATURES.keys())
    feature_keys = sorted(set(k for v in ITEM_FEATURES.values() for k in v))
    n_items = len(items)
    n_feats = len(feature_keys)
    feature_map = {k: i for i, k in enumerate(feature_keys)}
    feat_mat = [[0.0] * n_feats for _ in range(n_items)]
    for i, item in enumerate(items):
        for feat, val in ITEM_FEATURES[item].items():
            feat_mat[i][feature_map[feat]] = float(val)
    return items, feature_keys, feat_mat
