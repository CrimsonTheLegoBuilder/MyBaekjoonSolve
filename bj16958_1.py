import sys
from heapq import heappop, heappush


def dijkstra(p, d):
    if min_dist[p][d] < 3000:
        ans.append(min_dist[p][d])
    else:
        pq = []
        heappush(pq, (0, p))
        while pq:
            dist, cur = heappop(pq)
            if min_dist[p][cur] < dist:
                continue
            for w in range(N):
                if graph[cur][w]:
                    nxt_dist = dist + graph[cur][w]
                    if nxt_dist < min_dist[p][w]:
                        min_dist[p][w] = nxt_dist
                        heappush(pq, (nxt_dist, w))
        ans.append(min_dist[p][d])


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
min_dist = [[3000] * N for _ in range(N)]
for _ in range(M):
    s, e = map(int, sys.stdin.readline().strip().split())
    dijkstra(s-1, e-1)
for col in ans:
    print(col)
# for row in min_dist:
#     print(*row)
