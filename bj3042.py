import sys
from itertools import combinations


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


N = int(sys.stdin.readline().strip())
arr = [input() for _ in range(N)]
dots = []
for i in range(N):
    for j in range(N):
        if 'A' <= arr[i][j] <= 'Z':
            dots.append((i, j))
ans = 0
for a, b, c in combinations(range(len(dots)), 3):
    ans += (cross(dots[a], dots[b], dots[b], dots[c]) == 0)
print(ans)
