import sys
sys.setrecursionlimit(100000)


def dfs(r, c):
    global flag
    visited[r][c] = 1
    cur = grid[r][c]
    for x in range(8):
        nr = r + dr[x]
        nc = c + dc[x]
        if 0 <= nr < N and 0 <= nc < M and grid[nr][nc] == cur and not visited[nr][nc]:
            dfs(nr, nc)
        if 0 <= nr < N and 0 <= nc < M and grid[r][c] < grid[nr][nc]:
            flag = 0


N, M = map(int, sys.stdin.readline().strip().split())
grid = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
visited = [[0] * M for _ in range(N)]
dr = (1, 1, -1, -1, 0, 1, 0, -1)
dc = (1, -1, -1, 1, 1, 0, -1, 0)
ans = 0
for i in range(N):
    for j in range(M):
        if grid[i][j] and not visited[i][j]:
            flag = 1
            dfs(i, j)
            ans += flag
            # for u in range(N):
            #     print(*visited[u])
            # print()

print(ans)

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

4 4
0 0 0 1
0 0 1 0
0 1 0 0
1 0 0 0
'''
