from collections import deque


def bfs():
    while queue:
        i, j = queue.popleft()
        for k in range(4):
            ni = i + drc[k][0]
            nj = j + drc[k][1]
            if 0 <= ni < N and 0 <= nj < M and grid[ni][nj] == 'L' and not ~visited[ni][nj]:
                queue.append((ni, nj))
                visited[ni][nj] = visited[i][j] + 1


for tc in range(int(input())):
    N, M = map(int, input().split())
    grid = [str(input()) for _ in range(N)]
    visited = [[-1]*M for _ in range(N)]
    queue = deque()
    drc = ((1, 0), (0, 1), (-1, 0), (0, -1))
    for x in range(N):
        for y in range(M):
            if grid[x][y] == 'W':
                for z in range(4):
                    nx, ny = x + drc[z][0], y + drc[z][1]
                    if 0 <= nx < N and 0 <= ny < M and grid[nx][ny] == 'L':
                        queue.append((x, y))
                        visited[x][y] = 0
                        break
    bfs()
    ans = 0
    for x in range(N):
        for y in range(M):
            if ~visited[x][y]:
                ans += visited[x][y]
    # for row in visited:
    #     print(*row)
    print(f'#{tc+1} {ans}')
