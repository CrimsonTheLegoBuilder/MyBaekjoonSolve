import sys
from collections import deque


def bt(cnt, x):
    global ans
    if cnt == 3:
        for _ in range(N):
            for idx in visited_a:
                if idx:
                    ans += bfs(N, idx)
            forward()
        return
    for i in range(x, M):
        visited_a[i] = 1
        bt(cnt+1, i+1)
        visited_a[i] = 0


def bfs(n, x):
    visited = [[0] * M for _ in range(N)]
    queue.append((n, x))
    while queue:
        pass
    return 0


def forward():
    for j in range(M):
        if grid[N-1][j]:
            grid[N - 1][j] = 0
    for i in range(N, -1, -1):
        for j in range(M):
            if grid[i][j]:
                grid[i+1][j] = 1
                grid[i][j] = 0


N, M, D = map(int, sys.stdin.readline().strip().split())
grid = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
visited_a = [0] * M
queue = deque()
dr = (0, 0, -1)
dc = (1, -1, 0)
ans = 0
bt(0, 0)

