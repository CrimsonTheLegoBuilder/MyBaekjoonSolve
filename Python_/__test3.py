def get_answer(start):
    cur_cost[start] = 0
    visited = [1]+[0]*N

    visited[start] = 1

    for _ in range(N):

        for nxt in range(1, N + 1):
            if adj_m[start][nxt] == -1: continue
            if visited[nxt]: continue
            if cur_cost[nxt] <= cur_cost[start] + adj_m[start][nxt]: continue

            cur_cost[nxt] = cur_cost[start] + adj_m[start][nxt]

        min_value = 98765432198765432
        for nxt in range(1, N + 1):
            if visited[nxt]: continue
            if min_value <= cur_cost[nxt]: continue

            min_value = cur_cost[nxt]
            start = nxt

        visited[start] = 1


N = int(input())
M = int(input())
adj_m = [[-1]*(N + 1) for _ in range(N + 1)]
for _ in range(M):
    f, e, w = map(int, input().split())
    if adj_m[f][e] == -1 or adj_m[f][e] > w:
        adj_m[f][e] = w

s, E = map(int, input().split())

INT = 987654321987654321
cur_cost = [0]+[INT]*N

get_answer(s)

print(cur_cost[E])
