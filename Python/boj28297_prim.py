import sys
from math import asin, pi
from heapq import heappush as push
from heapq import heappop as pop
MAX = 4000


def cal_d(x1, y1, x2, y2):
    return (x1-x2)**2 + (y1-y2)**2


def cal_b(x1, y1, r1, x2, y2, r2):
    a = cal_d(x1, y1, x2, y2)
    if a <= (r1 + r2)**2:
        return 0
    b = (r1 - r2)**2
    c = a - b
    r = 2.0 * c**.5
    th = asin(c**.5/a**.5)
    if r1 < r2:
        r1, r2 = r2, r1
    r += r1*(pi-th)*2.0 + r2*th*2.0
    return r


def prim(v):
    t = 0
    cnt = 0
    mst = [0]*N
    push(H, (0, v))
    while H:
        cp, p = pop(H)
        if mst[p]:
            continue
        mst[p] = 1
        t += cp
        cnt += 1
        if cnt == N:
            return t
        for w in range(N):
            if graph[p][w] > -1 and not mst[w]:
                push(H, (graph[p][w], w))
    return t


N = int(sys.stdin.readline().strip())
gears = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
graph = [[-1]*N for _ in range(N)]
H = []
for i in range(N-1):
    xi, yi, ri = gears[i]
    for j in range(i+1, N):
        xj, yj, rj = gears[j]
        l_belt = cal_b(xi, yi, ri, xj, yj, rj)
        graph[i][j] = graph[j][i] = l_belt
print(prim(0))
# for row in graph:
#     print(*row)
