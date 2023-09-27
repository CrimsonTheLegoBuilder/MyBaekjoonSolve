import sys
sys.setrecursionlimit(10000)


def find_tile(p, x):  # (r, c) = start point
    r, c = p
    visited[r][c] = 1
    if x == '-':
        nr = r
        nc = c + 1
        if 0 <= nr < n and 0 <= nc < m and visited[nr][nc] != 1 and board[nr][nc] == '-':
            find_tile((nr, nc), x)
    elif x == '|':
        nr = r + 1
        nc = c
        if 0 <= nr < n and 0 <= nc < m and visited[nr][nc] != 1 and board[nr][nc] == '|':
            find_tile((nr, nc), x)
    return 0


n, m = map(int, sys.stdin.readline().strip().split())
board = [sys.stdin.readline().strip() for _ in range(n)]
visited = [[0] * m for _ in range(n)]

cnt = 0
for x in range(n):
    for y in range(m):
        if visited[x][y] != 1:
            find_tile((x, y), board[x][y])
            cnt += 1
print(cnt)
