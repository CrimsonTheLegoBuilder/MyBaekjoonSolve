def game(r, c, s):
    grid[r][c] = s
    for i in range(8):
        stack = []
        for l in range(1, N):
            nr = r + delta[i][0] * l
            nc = c + delta[i][1] * l
            if 0 <= nr < N and 0 <= nc < N and not grid[nr][nc]:
                break
            if 0 <= nr < N and 0 <= nc < N and grid[nr][nc] and grid[nr][nc] != s:
                stack.append((nr, nc))
            if 0 <= nr < N and 0 <= nc < N and grid[nr][nc] == s:
                while stack:
                    nr, nc = stack.pop()
                    grid[nr][nc] = s
                break


for tc in range(1, int(input())+1):
    N, M = map(int, input().split())
    grid = [[0] * N for _ in range(N)]
    grid[N//2][N//2] = 2
    grid[N//2-1][N//2] = 1
    grid[N//2][N//2-1] = 1
    grid[N//2-1][N//2-1] = 2
    delta = ((1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1))
    for _ in range(M):
        x, y, color = map(int, input().split())
        game(x-1, y-1, color)
    white = 0
    black = 0
    for row in grid:
        # print(*row)
        for i in row:
            black += (i == 1)
            white += (i == 2)
    print(f'#{tc}', black, white)
