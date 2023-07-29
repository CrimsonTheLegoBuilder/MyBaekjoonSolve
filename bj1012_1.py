import sys


def find_1(p):  # (r, c) = start point
    r, c = p
    dr = (-1, 0, 1, 0)  # CW
    dc = (0, 1, 0, -1)
    visited[r][c] = 1
    stack = [(r, c)]

    while stack:
        flag = True
        # print(stack[-1])
        r, c = stack[-1]

        for i in range(4):
            nr = r + dr[i]
            nc = c + dc[i]
            if 0 <= nr < n and 0 <= nc < m and visited[nr][nc] != 1 and board[nr][nc] == 1:
                flag = False
                stack.append((nr, nc))
                visited[nr][nc] = 1
                break
        if flag:
            stack.pop()
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
            if board[x][y] == 1 and visited[x][y] == 0:
                find_1((x, y))
                cnt += 1
    print(cnt)
