import sys
from collections import deque


def bfs(x, y, p):
    queue.append((x, y))
    visited_b[x][y] = p
    while queue:
        r, c = queue.popleft()
        for z in range(4):
            nr = r + dr[z]
            nc = c + dc[z]
            if 0 <= nr < N and 0 <= nc < M and not grid[nr][nc] and visited_b[nr][nc] != p:
                queue.append((nr, nc))
                visited_b[nr][nc] = p
            if 0 <= nr < N and 0 <= nc < M and grid[nr][nc]:
                stack.append((nr, nc))


def dfs(r, c, p):
    visited_d[r][c] = p
    for x in range(4):
        nr = r + dr[x]
        nc = c + dc[x]
        if 0 <= nr < N and 0 <= nc < M and grid[nr][nc] and visited_d[nr][nc] != p:
            dfs(nr, nc, p)


N, M = map(int, sys.stdin.readline().strip().split())
grid = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
visited_b = [[0] * M for _ in range(N)]
visited_d = [[0] * M for _ in range(N)]
queue = deque()
stack = []
ans = 0
dr8 = (1, 1, -1, -1, 0, 1, 0, -1)
dc8 = (1, -1, -1, 1, 1, 0, -1, 0)
dr = (1, 0, -1, 0)
dc = (0, 1, 0, -1)

for day in range(1, 11):
    cnt = 0
    for i in range(N):
        for j in range(M):
            if not grid[i][j] and visited_b[i][j] != day:
                bfs(i, j, day)
    while stack:
        i, j = stack.pop()
        if grid[i][j] > 0:
            grid[i][j] -= 1
    for i in range(N):
        for j in range(M):
            if grid[i][j] and visited_d[i][j] != day:
                dfs(i, j, day)
                cnt += 1
    for row in grid:
        print(*row)
    print()
    if cnt > 1:
        ans = day
        break
print(ans)

'''
8 10
4 3 2 2 1 2 5 5 6 6
3 3 3 2 1 2 4 5 5 6
2 2 2 2 1 2 3 3 3 3
2 1 4 3 3 2 2 3 4 5
1 1 0 3 2 3 2 2 2 3
0 0 0 1 2 2 0 1 1 1
0 1 2 1 1 1 0 1 0 1
0 1 1 1 2 1 0 0 0 0

8 10
1 2 3 2 1 2 5 5 6 6
2 2 3 2 1 2 4 5 5 6
3 3 3 2 1 2 3 3 3 3
3 2 1 1 1 2 2 3 4 5
2 1 0 0 0 1 2 2 2 3
1 0 0 1 1 1 0 1 1 1
0 0 2 1 1 1 0 1 0 1
0 1 1 1 2 1 0 0 0 0
'''
