import sys
from math import pi

X, Y = map(int, sys.stdin.readline().strip().split())
roof = [[0]*(X+1) for _ in range(Y+1)]
N = int(sys.stdin.readline().strip())
for i in range(N):
    x, y = map(int, sys.stdin.readline().strip().split())
    roof[y][x] = 1

ans = 0.0
for j in range(Y):
    for i in range(X):
        u, l, r, d = roof[j][i], roof[j+1][i], roof[j][i+1], roof[j+1][i+1]
        if u + l + r + d >= 3:
            ans += 1.0
        if u + l + r + d == 2:
            if u & d | r & l:
                ans += 1.0
            else:
                ans += pi/6 + 3**.5/4
        if u + l + r + d == 1:
            ans += pi/4
        else:
            continue
print(ans)
