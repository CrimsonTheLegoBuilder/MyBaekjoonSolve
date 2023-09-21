from heapq import heappush as h_push
from heapq import heappop as h_pop
import sys


def dijkstra(s):
    h_push(H, (0, s))
    dist[s] = 0
    while H:
        w_, p = h_pop(H)
        if dist[p] < w_:
            continue
        for nxt, w in graph[p]:
            n_w = w_ + w
            if dist[nxt] <= n_w:
                continue
            dist[nxt] = n_w
            h_push(H, (n_w, nxt))


n, m = map(int, sys.stdin.readline().strip().split())
graph = [[] for _ in range(n)]
H = []
total = 0
for _ in range(m):
    f, t, W = map(int, sys.stdin.readline().strip().split())
    graph[f].append([t, W])

INF = int(12e12)
dist = [INF] * n
dijkstra(0)
print(dist)


'''
7 11
0 1 32
0 2 31
0 5 60
0 6 51
1 2 21
2 4 46
2 6 25
3 4 34
3 5 18
4 5 40
4 6 51
'''