import sys
from collections import deque


def bfs(v):
    queue.append(v)
    visited[v] = 1
    cnt = 0
    while queue:
        p = queue.popleft()
        for w in graph[p]:
            if not visited[w]:
                visited[w] = 1
                cnt += 1
                queue.append(w)
    return cnt


n = int(sys.stdin.readline().strip())
k = int(sys.stdin.readline().strip())
graph = [set() for _ in range(n+1)]
visited = [0] * (n+1)
for _ in range(k):
    a, b = map(int, sys.stdin.readline().strip().split())
    graph[a].add(b)
    graph[b].add(a)
queue = deque()
print(bfs(1))
