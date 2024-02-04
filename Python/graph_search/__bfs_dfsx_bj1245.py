import sys
from collections import deque


def bfs(x, y):
    global cnt
    queue.append((x, y))
    visited[x][y] = 1
    flag = 1
    cur = grid[x][y]
    while queue:
        r, c = queue.popleft()
        for z in range(8):
            nr = r + dr[z]
            nc = c + dc[z]
            if 0 <= nr < N and 0 <= nc < M and grid[nr][nc] == cur and not visited[nr][nc]:
                queue.append((nr, nc))
                visited[nr][nc] = 1
            if 0 <= nr < N and 0 <= nc < M and grid[nr][nc] > cur:
                flag = 0
        # print(queue)
    if flag:
        cnt += 1


N, M = map(int, sys.stdin.readline().strip().split())
grid = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
visited = [[0] * M for _ in range(N)]
queue = deque()
dr = (1, 1, -1, -1, 0, 1, 0, -1)
dc = (1, -1, -1, 1, 1, 0, -1, 0)
cnt = 0
for i in range(N):
    for j in range(M):
        if grid[i][j] and not visited[i][j]:
            bfs(i, j)
            # for u in range(N):
            #     print(*visited[u])

print(cnt)


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

8 10
1 2 3 2 1 2 5 5 6 6
2 2 3 2 1 2 4 5 5 6
3 3 3 2 1 2 3 3 3 3
3 2 1 1 1 2 2 3 4 5
2 1 0 0 0 1 2 2 2 3
1 0 0 1 1 1 0 1 1 1
0 0 2 1 1 1 0 1 0 1
0 1 1 1 2 1 0 0 0 0
'''
