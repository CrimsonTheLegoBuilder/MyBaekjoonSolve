from heapq import heappop as h_pop
from heapq import heappush as h_push
MAX = 10001


def dijkstra(r, c):
    h_push(H, (0, (r, c)))
    cost[r][c] = 0
    while H:
        c, p = h_pop(H)
        i, j = p
        if cost[i][j] < c:
            continue
        for k in range(4):
            ni = i + drc[k][0]
            nj = j + drc[k][1]
            if 0 <= ni < N and 0 <= nj < N:
                c_ = 1 + abs(graph[ni][nj] - graph[i][j])*(graph[ni][nj] > graph[i][j])
                if cost[ni][nj] > c + c_:
                    cost[ni][nj] = c + c_
                    h_push(H, (c + c_, (ni, nj)))


for tc in range(int(input())):
    N = int(input())
    graph = [tuple(map(int, input().split())) for _ in range(N)]
    H = []
    cost = [[MAX]*N for _ in range(N)]
    drc = ((1, 0), (0, 1), (-1, 0), (0, -1))
    dijkstra(0, 0)
    print(f"#{tc+1} {cost[N-1][N-1]}")
