import sys


def dfs(r, c):
    global cnt
    cnt += 1
    visited[r][c] = 1
    color = grid[r][c]
    for x in range(4):
        nr = r + dr[x]
        nc = c + dc[x]
        if 0 <= nr < N and 0 <= nc < M and grid[nr][nc] == color and not visited[nr][nc]:
            dfs(nr, nc)


M, N = map(int, sys.stdin.readline().strip().split())
grid = [str(sys.stdin.readline().strip()) for _ in range(N)]
visited = [[0] * M for _ in range(N)]
power_w = 0
power_b = 0
dr = (1, -1, 0, 0)
dc = (0, 0, 1, -1)
for i in range(N):
    for j in range(M):
        cnt = 0
        if not visited[i][j]:
            dfs(i, j)
            # print(visited)
            if grid[i][j] == 'W':
                power_w += cnt**2
            else:
                power_b += cnt**2
print(power_w, power_b)

'''
5 6
WBWWWW
WWWWWW
BBBBBB
BBBWWB
WWWWWB
'''