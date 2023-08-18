import sys
sys.stdin = open('input_maze.txt', 'r')
from collections import deque


def bfs(r, c):
    queue.append((r, c))
    visited[r][c] = 1
    while queue:
        r, c = queue.popleft()
        for k in range(4):
            nr = r + dr[k]
            nc = c + dc[k]
            if 0 <= nr < 100 and 0 <= nc < 100 and maze[nr][nc] == 3:
                return 1
            if 0 <= nr < 100 and 0 <= nc < 100 and not maze[nr][nc] and not visited[nr][nc]:
                queue.append((nr, nc))
                visited[nr][nc] = 1
    return 0


for t in range(1, 11):
    _ = int(input())
    maze = [list(map(int, list(input()))) for _ in range(100)]
    visited = [[0]*100 for _ in range(100)]
    queue = deque()
    dr = (1, 0, -1, 0)
    dc = (0, 1, 0, -1)
    flag = 0
    for i in range(100):
        for j in range(100):
            if maze[i][j] == 2:
                print(f'#{t} {bfs(i, j)}')
                flag = 1
                break
        if flag:
            break
