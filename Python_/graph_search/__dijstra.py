# from heapq import heappush as h_push
# from heapq import heappop as h_pop
# import sys
#
#
# def dijkstra(s):
#     h_push(H, (0, s))
#     dist[s] = 0
#     while H:
#         w_, p = h_pop(H)
#         if dist[p] < w_:
#             continue
#         for nxt, w in graph[p]:
#             n_w = w_ + w
#             if dist[nxt] <= n_w:
#                 continue
#             dist[nxt] = n_w
#             h_push(H, (n_w, nxt))
#
#
# n, m = map(int, sys.stdin.readline().strip().split())
# graph = [[] for _ in range(n)]
# H = []
# total = 0
# for _ in range(m):
#     f, t, W = map(int, sys.stdin.readline().strip().split())
#     graph[f].append([t, W])
#
# INF = int(12e12)
# dist = [INF] * n
# dijkstra(0)
# print(dist)
#
#
# '''
# 7 11
# 0 1 32
# 0 2 31
# 0 5 60
# 0 6 51
# 1 2 21
# 2 4 46
# 2 6 25
# 3 4 34
# 3 5 18
# 4 5 40
# 4 6 51
# '''

data = [
    [0, 1, 12],
    [0, 2, 15],
    [1, 3, 4],
    [1, 4, 10],
    [2, 5, 7],
    [2, 6, 21],
    [3, 4, 3],
    [3, 7, 13],
    [4, 5, 10],
    [5, 8, 9],
    [5, 9, 19],
    [6, 9, 25],
    [7, 8, 15],
    [8, 9, 5],
]


def dijkstra(s):
    # 시작점 설정
    # total = 0
    D[s] = 0
    # 정점의 갯수만큼 선택하기
    for i in range(V + 1):
        # 가중치 최소값 찾기
        min_v = 987654321
        for j in range(V + 1):
            if vstd[j] == 0 and D[j] < min_v:
                min_v = D[j]
                v = j  # 정점 선택
        # 방문처리(선택)
        vstd[v] = 1
        # total += adj[PI[v]][v]
        # 인접 정점의 가중치 갱신
        for w in range(V + 1):
            if adj[v][w] and not vstd[w]:
                # if D[w] > adj[v][w]:
                #     D[w] = adj[v][w]
                #     PI[w] = v
                if D[w] > D[v] + adj[v][w]:  # 여기만 달라짐
                    D[w] = D[v] + adj[v][w]

    # return total


V, E = 9, 14

adj = [[0] * (V + 1) for _ in range(V + 1)]
vstd = [0] * (V + 1)
D = [987654321] * (V + 1)
# PI = list(range(V+1))

for i in range(E):
    s, e, w = data[i]
    adj[s][e] = adj[e][s] = w

dijkstra(0)
print(D)
print(sum(D))