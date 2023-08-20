import sys
from collections import deque


def bt(cnt, x):
    global ans
    if cnt == 3:
        temp = [row[:] for row in grid]
        kill = 0
        # for row in temp:
        #     print(*row)
        # print()
        for _ in range(N):
            for idx in range(M):
                if visited_a[idx]:
                    kill += bfs(temp, N, idx, -1)
            forward(temp)
        ans = max(ans, kill)
        return
    for i in range(x, M):
        visited_a[i] = 1
        bt(cnt+1, i+1)
        visited_a[i] = 0
    return


def bfs(temp, n, x, v):
    queue = deque()
    visited = [[0] * M for _ in range(N)]
    queue.append((n, x, v))
    while queue:
        r, c, v = queue.popleft()
        for i in range(3):
            nr = r + dr[i]
            nc = c + dc[i]
            if 0 <= nr < N and 0 <= nc < M and v < D:
                if temp[nr][nc]:
                    temp[nr][nc] = 0
                    return 1
                visited[nr][nc] = 1 if not ~v else visited[r][c] + 1
                queue.append((nr, nc, visited[nr][nc]))
    return 0


def forward(temp):
    for j in range(M):
        if temp[N-1][j]:
            temp[N-1][j] = 0
    for i in range(N-2, -1, -1):
        for j in range(M):
            if temp[i][j]:
                temp[i+1][j] = 1
                temp[i][j] = 0


N, M, D = map(int, sys.stdin.readline().strip().split())
grid = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
visited_a = [0] * M
dr = (0, 0, -1)
dc = (-1, 1, 0)
ans = 0
bt(0, 0)
print(ans)
