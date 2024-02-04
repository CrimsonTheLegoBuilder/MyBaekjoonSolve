import sys
from collections import deque


def dfs(v):
    print(v, end=' ')
    visited[v] = 1
    for w in range(1, N+1):
        if graph[v][w] and not visited[w]:
            visited[w] = 1
            dfs(w)


def bfs(v):
    queue.append(v)
    visited[v] = 1
    while queue:
        p = queue.popleft()
        print(p, end=' ')
        for w in range(1, N+1):
            if graph[p][w] and not visited[w]:
                visited[w] = 1
                queue.append(w)


N, M, V = map(int, sys.stdin.readline().strip().split())
graph = [[0]*(N+1) for _ in range(N+1)]
visited = [0] * (N+1)
for _ in range(M):
    a, b = map(int, sys.stdin.readline().strip().split())
    graph[a][b] = 1
    graph[b][a] = 1
queue = deque()
dfs(V)
print()
visited = [0] * (N+1)
bfs(V)
