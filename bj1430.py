import sys
from collections import deque

N, R, D, X, Y = map(int, sys.stdin.readline().strip().split())
coord = [(X, Y)] + [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
graph = [list() for _ in range(N + 1)]
visited = [0] * (N + 1)
depth = [0] * (N + 1)
damage = [0] * N
q = deque([0])

for i in range(N + 1):
    for j in range(1, N + 1):
        if i == j:
            continue
        if (coord[i][0] - coord[j][0])**2 + (coord[i][1] - coord[j][1])**2 <= R**2:
            graph[i].append(j)

# print(graph)

while q:
    k = q.popleft()
    for l in graph[k]:
        if visited[l] == 1:
            continue
        else:
            q.append(l)
            visited[l] = 1
            depth[l] = depth[k] + 1

ans = 0
# print(depth)
for i in depth[1:]:
    if i == 0:
        continue
    damage[i - 1] += 1

# print(damage)

for i in range(N):
    ans += D * damage[i] / 2**i

print(ans)
