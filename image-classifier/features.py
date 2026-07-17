import math

IMG_SIZE = 8


def extract_pixels(image):
    return [pixel for row in image for pixel in row]


def extract_edge_features(image):
    features = []
    for y in range(IMG_SIZE):
        for x in range(IMG_SIZE):
            gx = image[y][min(x + 1, IMG_SIZE - 1)] - image[y][max(x - 1, 0)]
            gy = image[min(y + 1, IMG_SIZE - 1)][x] - image[max(y - 1, 0)][x]
            mag = min(1.0, math.sqrt(gx*gx + gy*gy) / 2.0)
            features.append(mag)
    return features


def extract_symmetry(image):
    h_sym = 0.0
    for y in range(IMG_SIZE):
        for x in range(IMG_SIZE // 2):
            h_sym += abs(image[y][x] - image[y][IMG_SIZE - 1 - x])
    h_sym /= (IMG_SIZE * (IMG_SIZE // 2))

    v_sym = 0.0
    for y in range(IMG_SIZE // 2):
        for x in range(IMG_SIZE):
            v_sym += abs(image[y][x] - image[IMG_SIZE - 1 - y][x])
    v_sym /= ((IMG_SIZE // 2) * IMG_SIZE)

    return [1.0 - h_sym, 1.0 - v_sym]


def extract_density(image):
    total = sum(sum(row) for row in image)
    return [total / (IMG_SIZE * IMG_SIZE)]


def extract_all(image):
    feats = []
    feats.extend(extract_pixels(image))
    feats.extend(extract_edge_features(image))
    feats.extend(extract_symmetry(image))
    feats.extend(extract_density(image))
    return feats
