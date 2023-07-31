import sys
from itertools import combinations_with_replacement

n = int(sys.stdin.readline().strip())
dots = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
ans = int(9e12)

Y = sorted(list(set(d[1] for d in dots)))
# for a, b in combinations(tuple(range(n)), 3):
for y1, y2 in combinations_with_replacement(Y, 2):
    X = sorted([d[0] for d in dots if y1 <= d[1] <= y2])
    for i in range(len(X) - n//2 + 1):
        ans = min(ans, (y2 - y1 + 2) * (X[i + n//2 - 1] - X[i] + 2))

print(ans)
