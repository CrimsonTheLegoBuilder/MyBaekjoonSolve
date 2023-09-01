# =========== TIME OUT ============ #
import sys
sys.setrecursionlimit(10000)


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def monotone_chain(arr):  # get hull / sorting hull CCW
    arr.sort(key=lambda x: (x[0], x[1]))
    if len(arr) <= 2:
        return arr
    lower = []
    for dl in arr:
        while len(lower) > 1 and cross(lower[-2], lower[-1], lower[-1], dl) <= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(arr):
        while len(upper) > 1 and cross(upper[-2], upper[-1], upper[-1], du) <= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


def f(s, e, k):
    if s + 1 >= e or k == 0:
        return 0
    if dp[k][s][e] > -1:
        return dp[k][s][e]

    area = 0.0
    for m in range(s+1, e):
        area = max(cross(hull[s], hull[m], hull[m], hull[e]) + f(m, e, k-1), area)
    dp[k][s][e] = area
    return area


N, M = map(int, sys.stdin.readline().strip().split())
dots = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
hull = monotone_chain(dots)
dp = [[[-1]*len(hull) for _ in range(len(hull))] for _ in range(M)]
ans = 0
for i in range(len(hull)):
    for j in range(i+1, len(hull)):
        ans = max(ans, f(i, j, M-2))
print(ans/2)
