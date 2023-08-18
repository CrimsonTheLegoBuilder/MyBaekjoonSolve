import sys
from math import pi

X, Y = map(int, sys.stdin.readline().strip().split())
roof = [[0]*(X+1) for _ in range(Y+1)]
N = int(sys.stdin.readline().strip())
for i in range(N):
    x, y = map(int, sys.stdin.readline().strip().split())
    roof[y][x] = 1

ans = 0.0
for i in range(Y):
    for j in range(X):
        if roof[i][j]:
            if roof[i+1][j+1]:
                ans += 1.0
            elif not roof[i+1][j+1]:
                if not roof[i+1][j] and not roof[i][j+1]:
                    ans += pi/4
                elif roof[i+1][j] and roof[i][j+1]:
                    ans += 1.0
                else:
                    ans += pi/6 + 3**.5/4
        elif not roof[i][j]:
            if not roof[i+1][j+1]:
                if not roof[i+1][j] and not roof[i][j+1]:
                    continue
                elif roof[i+1][j] and roof[i][j+1]:
                    ans += 1.0
                else:
                    ans += pi/4
            elif roof[i+1][j+1]:
                if not roof[i+1][j] and not roof[i][j+1]:
                    ans += pi/4
                else:
                    ans += pi/6 + 3**.5/4
print(ans)
