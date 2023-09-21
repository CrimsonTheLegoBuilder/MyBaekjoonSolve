from heapq import heappop as h_pop
from heapq import heappush as h_push
MAX = 10001


def dijkstra(v):
    h_push(H, (0, v))
    cost[v] = 0
    while H:
        c, p = h_pop(H)
        if cost[p] < c:
            continue
        for w, c_ in graph[p]:
            if cost[w] > c + c_:
                cost[w] = c + c_
                h_push(H, (c + c_, w))


for tc in range(int(input())):
    N, E = map(int, input().split())
    graph = [[] for _ in range(N+1)]
    H = []
    for _ in range(E):
        s, e, W = map(int, input().split())
        graph[s].append([e, W])
    cost = [MAX] * (N+1)
    dijkstra(0)
    print(f"#{tc+1} {cost[N]}")
