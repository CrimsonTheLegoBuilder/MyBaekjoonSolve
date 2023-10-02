import sys
from heapq import heappush as push
from heapq import heappop as h_pop
MAX = 1_000_001


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


def dijkstra(v):
    cost[v] = 0
    push(H, (0, v))
    while H:
        cp, p = h_pop(H)
        if cost[p] < cp:
            continue
        if dots[p][1] == F:
            return int(cp+.5)
        for w, cw in graph[p]:
            ct = cp + cw
            if cost[w] > ct:
                cost[w] = ct
                push(H, (ct, w))
    return -1


N, F = map(int, sys.stdin.readline().strip().split())
dots = [(0, 0)]
graph = [[] for _ in range(N+1)]
cost = [MAX*5] * (N+1)
H = []
linked_dic = [dict() for _ in range(MAX)]
linked_dic[0][0] = 0
for i in range(1, N+1):
    x, y = map(int, sys.stdin.readline().strip().split())
    dots.append((x, y))
    linked_dic[x][y] = i
    # print(linked_dic[x])
for d in range(N+1):
    i, j = dots[d]
    for x in range(-2, 3, 1):
        for y in range(-2, 3, 1):
            if not x and not y:
                continue
            ni = i + x
            nj = j + y
            if 0 <= ni < MAX and 0 <= nj <= F:
                if linked_dic[ni].get(nj):
                    graph[d].append((linked_dic[ni][nj], cal_dist((i, j), (ni, nj))))
print(dijkstra(0))
# print(graph[:10])
# print(cost[:10])
# print(linked_dic[:10])
# print(graph[:10])
# print(cost[:10])


'''
5 3
1 2
6 3
4 1
3 2
0 2

'''