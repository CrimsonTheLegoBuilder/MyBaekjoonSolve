import sys
from collections import deque

m, n = map(int, sys.stdin.readline().strip().split())
board = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
# visited = [[0] * m for _ in range(n)]
q = deque()

for x in range(n):
    for y in range(m):
        if board[x][y] == 1:
            q.append((x, y))


def bfs():  # (r, c) = start point
    dr = (-1, 0, 1, 0)  # CW
    dc = (0, 1, 0, -1)
    # visited[r][c] = 1
    while q:
        r, c = q.popleft()
        for i in range(4):
            nr = r + dr[i]
            nc = c + dc[i]
            if 0 <= nr < n and 0 <= nc < m and not board[nr][nc]:  # and visited[nr][nc] != 1:
                board[nr][nc] = board[r][c] + 1
                q.append((nr, nc))
                # visited[nr][nc] = 1
    return


bfs()
ans = -1
for j in board:
    for k in j:
        if k == 0:
            print(-1)
            exit(0)
        if ans < k:
            ans = k

print(ans - 1)
