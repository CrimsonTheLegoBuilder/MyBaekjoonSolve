from heapq import heappush as h_push
from heapq import heappop as h_pop


def prim(v):
    total = 0
    h_push(H, (0, v))
    while H:
        c, p = h_pop(H)
        if mst[p]:
            continue
        mst[p] = 1
        total += c
        for w, c_ in graph[p]:
            if not mst[w]:
                h_push(H, (c_, w))
    return total


for tc in range(int(input())):
    V, E = map(int, input().split())
    graph = [[] for _ in range(V+1)]
    for _ in range(E):
        n1, n2, C = map(int, input().split())
        graph[n1].append([n2, C])
        graph[n2].append([n1, C])
    H = []
    mst = [0] * (V+1)
    print(f"#{tc+1} {prim(0)}")
