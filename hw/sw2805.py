from collections import deque


def bfs(r, c):
    global total
    queue.append((r, c))
    total += grid[r][c]
    visited[r][c] = 0
    while queue:
        r, c = queue.popleft()
        for i in range(4):
            nr = r + drc[i][0]
            nc = c + drc[i][1]
            if 0 <= nr < N and 0 <= nc < N and not ~visited[nr][nc]:
                if visited[r][c] == N // 2:
                    continue
                queue.append((nr, nc))
                visited[nr][nc] = visited[r][c] + 1
                total += grid[nr][nc]


for tc in range(1, int(input())+1):
    N = int(input())
    grid = [list(map(int, input())) for _ in range(N)]
    visited = [[-1]*N for _ in range(N)]
    drc = ((1, 0), (0, 1), (-1, 0), (0, -1))
    queue = deque()
    total = 0
    bfs(N//2, N//2)
    print(total)
