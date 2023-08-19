import sys
sys.setrecursionlimit(10000)


def dfs(r, c, d):
    global cnt
    drc = ((0, 1), (1, 1), (1, 0))
    if r == N-1 and c == N-1:
        cnt += 1
        return
    if d == 0:
        for i in range(2):
            nr = r + drc[i][0]
            nc = c + drc[i][1]
            if 0 <= nr < N and 0 <= nc < N:
                if i == 0 and not grid[r][nc] or \
                   i == 1 and not grid[r][nc] and not grid[nr][nc] and not grid[nr][c]:
                    dfs(nr, nc, i)
    if d == 1:
        for i in range(3):
            nr = r + drc[i][0]
            nc = c + drc[i][1]
            if 0 <= nr < N and 0 <= nc < N:
                if i == 0 and not grid[r][nc] or \
                   i == 1 and not grid[r][nc] and not grid[nr][nc] and not grid[nr][c] or \
                   i == 2 and not grid[nr][c]:
                    dfs(nr, nc, i)
    if d == 2:
        for i in range(1, 3):
            nr = r + drc[i][0]
            nc = c + drc[i][1]
            if 0 <= nr < N and 0 <= nc < N:
                if i == 1 and not grid[r][nc] and not grid[nr][nc] and not grid[nr][c] or \
                   i == 2 and not grid[nr][c]:
                    dfs(nr, nc, i)
    return


N = int(sys.stdin.readline().strip())
grid = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
cnt = 0
if grid[0][2] or grid[N-1][N-1]:
    print(0)
else:
    dfs(0, 1, 0)
    print(cnt)
