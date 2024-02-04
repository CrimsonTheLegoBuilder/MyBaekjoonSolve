import sys
from collections import deque


def bfs(v, target):
    queue.append(v)
    visited[v] = 1
    while queue:
        p = queue.popleft()
        for w in range(1, n+1):
            if w == p:
                continue
            if graph[p][w] == 1 and not visited[w]:
                if w == target:
                    queue.clear()
                    graph[v][w] = min(graph[v][w], visited[p])
                    graph[w][v] = min(graph[w][v], visited[p])
                    return visited[p]
                visited[w] = visited[p] + 1
                queue.append(w)
    # return 0


n, m = map(int, sys.stdin.readline().strip().split())
graph = [[int(12e12)]*(n+1) for _ in range(n+1)]
limit = int(12e12)
queue = deque()
for _ in range(m):
    a, b = map(int, sys.stdin.readline().strip().split())
    graph[a][b] = 1
    graph[b][a] = 1

min_bacon = int(12e12)
ans = 0
for i in range(1, n+1):
    for j in range(i+1, n+1):
        visited = [0] * (n + 1)
        # print(bfs(i, j))
        bfs(i, j)
    # print()
for row in graph:
    print(*row)
for r in range(1, n+1):
    total = 0
    for c in graph[r]:
        if c < limit:
            total += c
    if min_bacon > total:
        min_bacon = total
        ans = r
print(ans)
