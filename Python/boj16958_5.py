import sys


def cal_taxi_dist(x1, y1, x2, y2):
    return abs(x1-x2) + abs(y1-y2)


N, T = map(int, sys.stdin.readline().split())
info = []
special = set()
not_special = set()

for i in range(N):
    si, xi, yi = map(int, sys.stdin.readline().split())
    info.append((xi, yi))
    special.add(i) if si else not_special.add(i)

to_spec = [0]*N
for i in not_special:
    xi, yi = info[i]
    min_dist = 2001
    for j in special:
        xj, yj = info[j]
        min_dist = min(cal_taxi_dist(xi, yi, xj, yj), min_dist)
    to_spec[i] = min_dist

M = int(sys.stdin.readline())
for _ in range(M):
    s, e = map(int, sys.stdin.readline().split())
    dist = min(cal_taxi_dist(*info[s-1], *info[e-1]), to_spec[s-1] + T + to_spec[e-1])
    sys.stdout.write(f"{dist}\n")
