"""Lab 12.5: train a logistic-regression 'packed or not' model on dataset.csv; save it as JSON."""
import csv
import json

import numpy as np

rows = list(csv.reader(open("dataset.csv")))
names, rows = rows[0][3:], rows[1:]
X = np.array([[float(v) for v in r[3:]] for r in rows])
y = np.array([int(r[2]) for r in rows])
blob = np.array([int(r[1]) for r in rows])

rng = np.random.default_rng(5)
order = rng.permutation(len(rows))
train, test = order[:56], order[56:]                 # 70% / 30%

mean, std = X[train].mean(0), X[train].std(0) + 1e-9
Z = (X - mean) / std
w, b = np.zeros(Z.shape[1]), 0.0
for _ in range(2000):                                # plain gradient descent on log-loss
    p = 1 / (1 + np.exp(-(Z[train] @ w + b)))
    w -= 0.1 * Z[train].T @ (p - y[train]) / len(train)
    b -= 0.1 * float((p - y[train]).mean())

predict = lambda idx: (1 / (1 + np.exp(-(Z[idx] @ w + b))) >= 0.5).astype(int)
baseline = (X[:, 0] >= 7.2).astype(int)              # 'entropy >= 7.2 means packed'
print(f"baseline (entropy >= 7.2) on all 80: {np.mean(baseline == y):.0%} "
      f"(false positives on blob programs: {int(((baseline == 1) & (y == 0) & (blob == 1)).sum())})")
print(f"model on the {len(test)} held-out samples: {np.mean(predict(test) == y[test]):.0%}")
for n, wi in zip(names, w):
    print(f"  weight {wi:+.2f}  {n}")
json.dump({"features": names, "mean": mean.tolist(), "std": std.tolist(),
           "weights": w.tolist(), "bias": b}, open("model.json", "w"), indent=1)
