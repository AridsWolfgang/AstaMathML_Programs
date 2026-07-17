import math
from similarity import cosine_similarity, pearson_correlation


class CollaborativeFiltering:
    def __init__(self, ratings, mask, n_users, n_items):
        self.ratings = ratings
        self.mask = mask
        self.n_users = n_users
        self.n_items = n_items
        self.user_means = [
            sum(ratings[u][i] for i in range(n_items) if mask[u][i]) /
            max(sum(mask[u]), 1)
            for u in range(n_users)
        ]

    def _user_ratings_vector(self, user_idx):
        return self.ratings[user_idx]

    def _item_ratings_vector(self, item_idx):
        return [self.ratings[u][item_idx] for u in range(self.n_users)]

    def user_based_cf(self, target_user, target_item, k=5):
        target_vec = self._user_ratings_vector(target_user)
        similarities = []
        for other in range(self.n_users):
            if other == target_user:
                continue
            if not self.mask[other][target_item]:
                continue
            common = [i for i in range(self.n_items)
                      if self.mask[target_user][i] and self.mask[other][i]]
            if len(common) < 2:
                continue
            a = [target_vec[i] for i in common]
            b = [self.ratings[other][i] for i in common]
            sim = pearson_correlation(a, b)
            if sim > 0:
                similarities.append((sim, other))
        similarities.sort(reverse=True)
        neighbors = similarities[:k]
        if not neighbors:
            return self.user_means[target_user]
        num = 0.0
        den = 0.0
        for sim, other in neighbors:
            other_mean = self.user_means[other]
            num += sim * (self.ratings[other][target_item] - other_mean)
            den += abs(sim)
        if den == 0:
            return self.user_means[target_user]
        return self.user_means[target_user] + num / den

    def item_based_cf(self, target_user, target_item, k=5):
        similarities = []
        for other_item in range(self.n_items):
            if other_item == target_item:
                continue
            users_who_rated_both = [u for u in range(self.n_users)
                                    if self.mask[u][target_item] and self.mask[u][other_item]]
            if len(users_who_rated_both) < 2:
                continue
            a = [self.ratings[u][target_item] for u in users_who_rated_both]
            b = [self.ratings[u][other_item] for u in users_who_rated_both]
            sim = cosine_similarity(a, b)
            if sim > 0:
                similarities.append((sim, other_item))
        similarities.sort(reverse=True)
        neighbors = similarities[:k]
        if not neighbors:
            return self.user_means[target_user]
        num = 0.0
        den = 0.0
        for sim, other_item in neighbors:
            if self.mask[target_user][other_item]:
                num += sim * self.ratings[target_user][other_item]
                den += abs(sim)
        if den == 0:
            return self.user_means[target_user]
        return num / den

    def predict_all(self, method="user", k=5):
        preds = [[0.0] * self.n_items for _ in range(self.n_users)]
        for u in range(self.n_users):
            for i in range(self.n_items):
                if self.mask[u][i]:
                    preds[u][i] = self.ratings[u][i]
                else:
                    if method == "user":
                        preds[u][i] = self.user_based_cf(u, i, k)
                    else:
                        preds[u][i] = self.item_based_cf(u, i, k)
        return preds
