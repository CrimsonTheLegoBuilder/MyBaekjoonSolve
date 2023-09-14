import sys
from collections import deque


def bfs(r, c):
    queue.append((r, c))
    visited[r][c] = 1
    while queue:
        i, j = queue.popleft()
        for k in range(4):
            ni = i + drc[k][0]
            nj = j + drc[k][1]
            if 0 <= ni < N and 0 <= nj < M and not visited[ni][nj]:
                if not cheese[ni][nj]:
                    queue.append((ni, nj))
                    visited[ni][nj] = 1
                else:
                    surface.append((ni, nj))
                    visited[ni][nj] = 1
    return


def melt():
    total = 0
    for i, j in surface:
        cheese[i][j] = 0
        total += 1
    return total


N, M = map(int, sys.stdin.readline().strip().split())
cheese = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
drc = ((1, 0), (0, 1), (-1, 0), (0, -1))
queue = deque()

hour = 0
remain = 0
while True:
    visited = [[0]*M for _ in range(N)]
    surface = []
    bfs(0, 0)
    cnt = melt()
    if not cnt:
        print(hour)
        print(remain)
        break
    else:
        remain = cnt
        hour += 1
