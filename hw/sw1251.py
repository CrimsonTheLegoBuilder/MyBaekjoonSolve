import sys; sys.stdin = open("sw1251_input.txt", "r")
from heapq import heappop as h_pop
from heapq import heappush as h_push


def cal_dist_square(x1, y1, x2, y2):
    return (x1-x2)**2 + (y1-y2)**2


def prim(v):
    global total
    h_push(H, (0, v))
    total = 0
    cnt = 0
    while H:
        c, p = h_pop(H)
        if mst[p]:
            continue
        mst[p] = 1
        total += c
        cnt += 1
        if cnt == N:
            return
        for w in range(N):
            if graph[p][w] and not mst[w]:
                h_push(H, (graph[p][w], w))
    return


for tc in range(int(input())):
    N = int(input())
    cx = tuple(map(int, input().split()))
    cy = tuple(map(int, input().split()))
    tax = float(input())
    graph = [[0]*N for _ in range(N)]
    for i in range(N-1):
        for j in range(i+1, N):
            xi, yi, xj, yj = cx[i], cy[i], cx[j], cy[j]
            graph[i][j] = graph[j][i] = cal_dist_square(xi, yi, xj, yj)
    H = []
    mst = [0]*N
    total = 0
    prim(0)
    print(f"#{tc+1} {int(total*tax+.5)}")
