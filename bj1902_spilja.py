import sys


def meet(y, dot, slp):
    if slp[1] == 0:
        return
    x = slp[0] * (y - dot[1]) / slp[1] + dot[0]
    return x


n = int(sys.stdin.readline())

peak = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
slope = [(peak[i + 1][0] - peak[i][0], peak[i + 1][1] - peak[i][1]) for i in range(n - 1)]

bottom, top = 0, int(1e6)
while abs(top - bottom) > 0.00001:
    left, right = 0, peak[-1][0]
    mid = (bottom + top) / 2.0
    for i in range(n - 1):
        X = meet(mid, peak[i], slope[i])
        if X is None and peak[i][1] < mid:
            continue
        elif X is None and peak[i][1] > mid:
            left, right = peak[-1][0], 0
        elif slope[i][1] < 0 and X > left:
            left = X
        elif slope[i][1] > 0 and X < right:
            right = X
    if left > right:
        bottom = mid
    else:
        top = mid


print(round(top, 2))
