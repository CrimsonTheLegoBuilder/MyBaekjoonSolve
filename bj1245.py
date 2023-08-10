import sys


def dfs(r, c):
    global cnt
    cnt += 1
    visited[r][c] = 1
    cur = grid[r][c]
    for x in range(4):
        nr = r + dr[x]
        nc = c + dc[x]
        if 0 <= nr < N and 0 <= nc < M and grid[nr][nc] <= cur and not visited[nr][nc]:
            dfs(nr, nc)


N, M = map(int, sys.stdin.readline().strip().split())
grid = [list(map(int, sys.stdin.readline().strip())) for _ in range(N)]
visited = [[0] * M for _ in range(N)]
cnt = 0
dr = (1, -1, 0, 0)
dc = (0, 0, 1, -1)
for i in range(N):
    for j in range(M):
        if not visited[i][j]:

