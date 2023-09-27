import sys
# ======== CAUTION! ======== WRONG CODE! ======== #
# ======== CAUTION! ======== WRONG CODE! ======== #
# ======== CAUTION! ======== WRONG CODE! ======== #


def find_1(p):  # (r, c) = start point
    r, c = p
    dr = (-1, 0, 1, 0)  # CW
    dc = (0, 1, 0, -1)
    visited[r][c] = 1
    stack = [(r, c)]

    while stack:
        flag = True
        # print(stack[-1])

        for i in range(4):
            nr = r + dr[i]
            nc = c + dc[i]
            if 0 <= nr < n and 0 <= nc < m and visited[nr][nc] != 1 and board[nr][nc] == 1:
                flag = False
                stack.append((nr, nc))
                r, c = nr, nc
                visited[r][c] = 1
        if flag:
            r, c = stack.pop()
    return 0


t = int(sys.stdin.readline().strip())
for _ in range(t):
    m, n, k = map(int, sys.stdin.readline().strip().split())
    cab_arr = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(k)]
    board = [[0] * m for _ in range(n)]
    visited = [[0] * m for _ in range(n)]
    for (x, y) in cab_arr:
        board[y][x] = 1
    cnt = 0
    for x in range(n):
        for y in range(m):
            if board[x][y] == 1 and visited[x][y] == 0:
                find_1((x, y))
                cnt += 1
    print(cnt)

# ======== CAUTION! ======== WRONG CODE! ======== #
# ======== CAUTION! ======== WRONG CODE! ======== #
# ======== CAUTION! ======== WRONG CODE! ======== #
