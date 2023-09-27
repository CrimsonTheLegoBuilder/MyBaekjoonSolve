import sys
from collections import deque


def bfs(v, target):
    queue.append(v)
    visited[v] = 1
    while queue:
        p = queue.popleft()
        for w in range(1, n+1):
            if graph[p][w] and not visited[w]:
                if w == target:
                    queue.clear()
                    return visited[p] + 1
                visited[w] = visited[p] + 1
                queue.append(w)
    # return 0


n, m = map(int, sys.stdin.readline().strip().split())
graph = [[0]*(n+1) for _ in range(n+1)]
queue = deque()
for _ in range(m):
    a, b = map(int, sys.stdin.readline().strip().split())
    graph[a][b] = 1
    graph[b][a] = 1

min_bacon = int(12e12)
ans = 0
for i in range(1, n+1):
    bacon = 0
    for j in range(1, n+1):
        if i == j:
            continue
        visited = [0] * (n + 1)
        # print(bfs(i, j))
        bacon += bfs(i, j)
    if min_bacon > bacon:
        min_bacon = bacon
        ans = i
print(ans)
