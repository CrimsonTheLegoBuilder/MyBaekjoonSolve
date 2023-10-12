import sys
from heapq import heappush as push
from heapq import heappop as hp
INF = int(1e16)


def dijkstra(v, d):
    C[v] = 0
    push(H, (0, v))
    while H:
        cc, p = hp(H)
        if C[p] < cc:
            continue
        if p == d:
            return cc
        for w, cw in G[p]:
            c_nxt = cw + cc
            if C[w] > c_nxt:
                C[w] = c_nxt
                push(H, (c_nxt, w))


N = int(sys.stdin.readline().strip())
M = int(sys.stdin.readline().strip())
G = [[] for _ in range(N+1)]
for _ in range(M):
    s, e, c = map(int, sys.stdin.readline().strip().split())
    G[s].append([e, c])
s, e = map(int, sys.stdin.readline().strip().split())
C = [INF]*(N+1)
H = []
print(dijkstra(s, e))
