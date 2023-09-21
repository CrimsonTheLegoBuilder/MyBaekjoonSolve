from heapq import heappush as h_push
from heapq import heappop as h_pop
import sys


def prim(s):
    global total
    H = []
    mst = [0] * V
    h_push(H, (0, s))
    while H:
        w_, p = h_pop(H)
        if mst[p]:
            continue
        mst[p] = 1
        total += w_
        for w in range(V):
            if not graph[p][w] or mst[w]:
                continue
            h_push(H, (graph[p][w], w))


V, E = map(int, sys.stdin.readline().strip().split())
graph = [[0]*V for _ in range(E)]
total = 0
for _ in range(E):
    f, t, W = map(int, sys.stdin.readline().strip().split())
    graph[f][t] = W
    graph[t][f] = W
prim(0)
print(total)


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