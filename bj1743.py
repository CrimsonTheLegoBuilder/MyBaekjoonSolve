import sys
sys.setrecursionlimit(10000)


def dfs(r, c):
    global cnt
    cnt += 1
    visited[r][c] = 1
    for o in range(4):
        nr = r + dr[o]
        nc = c + dc[o]
        if 0 <= nr < N and 0 <= nc < M and grid[nr][nc] and not visited[nr][nc]:
            dfs(nr, nc)


N, M, K = map(int, sys.stdin.readline().strip().split())
visited = [[0] * M for _ in range(N)]
grid = [[0] * M for _ in range(N)]
for _ in range(K):
    x, y = map(int, sys.stdin.readline().strip().split())
    grid[x-1][y-1] = 1
dr = (1, -1, 0, 0)
dc = (0, 0, 1, -1)
largest = 0
for i in range(N):
    for j in range(M):
        cnt = 0
        if grid[i][j] and not visited[i][j]:
            dfs(i, j)
            largest = max(largest, cnt)
print(largest)
