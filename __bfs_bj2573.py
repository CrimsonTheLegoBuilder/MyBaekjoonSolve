import sys
from collections import deque


def bfs(x, y, p):
    queue.append((x, y))
    visited[x][y] = p
    while queue:
        r, c = queue.popleft()
        for z in range(8):
            nr = r + dr8[z]
            nc = c + dc8[z]
            if 0 <= nr < N and 0 <= nc < M and not grid[nr][nc] and visited[nr][nc] != p:
                queue.append((nr, nc))
                visited[nr][nc] = p
            if 0 <= nr < N and 0 <= nc < M and grid[nr][nc]:
                grid[nr][nc] -= 1


def dfs(r, c):
    visited[r][c] = 0
    cur = grid[r][c]
    for x in range(4):
        nr = r + dr4[x]
        nc = c + dc4[x]
        if 0 <= nr < N and 0 <= nc < M and grid[nr][nc] == cur and visited[nr][nc]:
            for y in range(8):
                flag = 1
                tr = nr + dr4[y]
                tc = nc + dc4[y]
                if 0 <= tr < N and 0 <= tc < M and grid[tr][tc] < grid[nr][nc]:
                    flag = 0
                if flag:
                    dfs(nr, nc)


N, M = map(int, sys.stdin.readline().strip().split())
grid = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
visited = [[0] * M for _ in range(N)]
queue = deque()
dr8 = (1, 1, -1, -1, 0, 1, 0, -1)
dc8 = (1, -1, -1, 1, 1, 0, -1, 0)
dr4 = (1, 0, -1, 0)
dc4 = (0, 1, 0, -1)

for day in range(1, 11):
    for i in range(N):
        for j in range(M):
            if not grid[i][j] and visited[i][j] != day:
                bfs(i, j, day)


'''
8 10
4 3 2 2 1 2 5 5 6 6
3 3 3 2 1 2 4 5 5 6
2 2 2 2 1 2 3 3 3 3
2 1 1 1 1 2 2 3 4 5
1 1 0 0 0 1 2 2 2 3
0 0 0 1 1 1 0 1 1 1
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
