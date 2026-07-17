import math
import random

IMG_SIZE = 8


def blank():
    return [[0] * IMG_SIZE for _ in range(IMG_SIZE)]


def add_noise(img, intensity=0.1):
    return [
        [min(1.0, max(0.0, v + random.gauss(0, intensity))) for v in row]
        for row in img
    ]


def draw_circle():
    img = blank()
    cx = cy = (IMG_SIZE - 1) / 2
    r = IMG_SIZE / 2 - 0.8
    for y in range(IMG_SIZE):
        for x in range(IMG_SIZE):
            d = math.sqrt((x - cx)**2 + (y - cy)**2)
            if abs(d - r) < 1.0:
                img[y][x] = 1.0
    return img


def draw_square():
    img = blank()
    margin = 1.5
    for y in range(IMG_SIZE):
        for x in range(IMG_SIZE):
            if (margin <= y < IMG_SIZE - margin) and (margin <= x < IMG_SIZE - margin):
                if (y < margin + 0.5 or y >= IMG_SIZE - margin - 0.5 or
                    x < margin + 0.5 or x >= IMG_SIZE - margin - 0.5):
                    img[y][x] = 1.0
    return img


def draw_triangle():
    img = blank()
    for y in range(IMG_SIZE):
        for x in range(IMG_SIZE):
            top = IMG_SIZE * 0.15
            bot = IMG_SIZE * 0.85
            left = IMG_SIZE * 0.1 + (y / IMG_SIZE) * (IMG_SIZE * 0.4)
            right = IMG_SIZE * 0.9 - (y / IMG_SIZE) * (IMG_SIZE * 0.4)
            if top <= y <= bot and left <= x <= right:
                if (abs(x - left) < 0.7 or abs(x - right) < 0.7 or
                    abs(y - bot) < 0.7):
                    img[y][x] = 1.0
    return img


def draw_cross():
    img = blank()
    cx = cy = (IMG_SIZE - 1) / 2
    thickness = 1.2
    for y in range(IMG_SIZE):
        for x in range(IMG_SIZE):
            h_dist = abs(y - cy)
            v_dist = abs(x - cx)
            if (v_dist <= thickness and h_dist <= IMG_SIZE / 2 - 0.5):
                img[y][x] = 1.0
            if (h_dist <= thickness and v_dist <= IMG_SIZE / 2 - 0.5):
                img[y][x] = 1.0
    return img


SHAPE_FNS = {
    "circle": draw_circle,
    "square": draw_square,
    "triangle": draw_triangle,
    "cross": draw_cross,
}

SHAPE_LABELS = ["circle", "square", "triangle", "cross"]


def generate_dataset(samples_per_class=40, noise=0.15, seed=42):
    random.seed(seed)
    X, y = [], []
    for label, shape_fn in SHAPE_FNS.items():
        for _ in range(samples_per_class):
            img = shape_fn()
            img = add_noise(img, noise)
            X.append(img)
            y.append(label)
    combined = list(zip(X, y))
    random.shuffle(combined)
    X, y = zip(*combined)
    return list(X), list(y)


def flatten(images):
    return [[pixel for row in img for pixel in row] for img in images]


def print_image(img):
    for row in img:
        line = "".join("##" if v > 0.3 else ".." if v > 0.05 else "  " for v in row)
        print(f"    {line}")
