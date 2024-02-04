import sys
sys.setrecursionlimit(10000)


def dfs_b(r, c):
    visitedB[r][c] = 1
    color = grid[r][c]
    for i in range(4):
        nr = r + dr[i]
        nc = c + dc[i]
        if 0 <= nr < N and 0 <= nc < N and grid[nr][nc] == color and not visitedB[nr][nc]:
            dfs_b(nr, nc)
    return


def dfs_rg(r, c):
    visitedRG[r][c] = 1
    color = grid[r][c]
    for i in range(4):
        nr = r + dr[i]
        nc = c + dc[i]
        if 0 <= nr < N and 0 <= nc < N and not visitedRG[nr][nc]:
            if color == 'B' and grid[nr][nc] == 'B':
                dfs_rg(nr, nc)
            if color != 'B' and grid[nr][nc] != 'B':
                dfs_rg(nr, nc)
    return


N = int(sys.stdin.readline().strip())
grid = tuple(str(sys.stdin.readline().strip()) for _ in range(N))
visitedB = [[0] * N for _ in range(N)]
visitedRG = [[0] * N for _ in range(N)]
dr = (-1, 0, 1, 0)
dc = (0, -1, 0, 1)
cnt_b = 0
cnt_rg = 0

for j in range(N):
    for k in range(N):
        if not visitedB[j][k]:
            dfs_b(j, k)
            cnt_b += 1
        if not visitedRG[j][k]:
            dfs_rg(j, k)
            cnt_rg += 1

print(cnt_b, cnt_rg)
