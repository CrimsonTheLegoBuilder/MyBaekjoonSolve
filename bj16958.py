import sys
from heapq import heappop, heappush


def dijkstra(p, d):
    min_dist = [3000] * N
    min_dist[p] = 0
    pq = []
    heappush(pq, (p, 0))
    while pq:
        cur, dist = heappop(pq)
        if min_dist[cur] < dist:
            continue
        for w in range(N):
            if graph[cur][w]:
                nxt_dist = dist + graph[cur][w]
                if nxt_dist < min_dist[w]:
                    min_dist[w] = nxt_dist
                    heappush(pq, (w, nxt_dist))
    ans.append(min_dist[d])


N, T = map(int, sys.stdin.readline().strip().split())
graph = [[0]*N for _ in range(N)]
info = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
for i in range(N):
    si, xi, yi = info[i]
    for j in range(i+1, N):
        sj, xj, yj = info[j]
        tmp = abs(xi-xj)+abs(yi-yj)
        if si and sj and tmp > T:
            tmp = T
        graph[i][j] = tmp
        graph[j][i] = tmp
# for row in graph:
#     print(*row)
M = int(sys.stdin.readline().strip())
ans = []
for _ in range(M):
    s, e = map(int, sys.stdin.readline().strip().split())
    dijkstra(s-1, e-1)
for col in ans:
    print(col)
