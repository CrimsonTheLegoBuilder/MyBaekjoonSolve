import sys
from heapq import heappop as h_pop
from heapq import heappush as push
MAX = int(1e16)


def dijkstra(v):
    C[v] = 0
    push(H, (0, 0, v))
    while H:
        cur, pre, p = h_pop(H)
        if C[p] < cur:
            continue
        P[p] = pre
        if p == e:
            return cur
        for w, cw in G[p]:
            if C[w] > cur + cw:
                C[w] = cur + cw
                push(H, (C[w], p, w))
    return -1


N = int(sys.stdin.readline().strip())
M = int(sys.stdin.readline().strip())
G = [[] for _ in range(N + 1)]
for _ in range(M):
    s, e, c = map(int, sys.stdin.readline().strip().split())
    G[s].append([e, c])
# for i in range(N):
#     G[i+1].sort(key=lambda o: o[1])
s, e = map(int, sys.stdin.readline().strip().split())
C = [MAX] * (N + 1)
H = []
P = [0] * (N+1)
print(dijkstra(s))
# print(C)
# print(P)
cnt = 1
x = e
seq = [e]
while x != s:
    x = P[x]
    seq.append(x)
    cnt += 1
print(cnt)
print(*seq[::-1])
