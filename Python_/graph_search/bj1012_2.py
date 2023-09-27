import sys
sys.setrecursionlimit(10000)


def find_1(p):  # (r, c) = start point
    r, c = p
    dr = (-1, 0, 1, 0)  # CW
    dc = (0, 1, 0, -1)
    visited[r][c] = 1

    for i in range(4):
        nr = r + dr[i]
        nc = c + dc[i]
        if 0 <= nr < n and 0 <= nc < m and visited[nr][nc] != 1 and board[nr][nc] == 1:
            find_1((nr, nc))

    return 0


t = int(sys.stdin.readline().strip())
for _ in range(t):
    m, n, k = map(int, sys.stdin.readline().strip().split())
    board = [[0] * m for _ in range(n)]
    visited = [[0] * m for _ in range(n)]
    for _ in range(k):
        y, x = map(int, sys.stdin.readline().strip().split())
        board[x][y] = 1

    cnt = 0
    for x in range(n):
        for y in range(m):
            if board[x][y] == 1 and visited[x][y] != 1:
                find_1((x, y))
                cnt += 1
    print(cnt)
