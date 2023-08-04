def dfs(r, c, x):
    visited[r][c] = x + 1
    if r == 99:
        return
    dc = [-1, 1]
    for i in range(2):
        nc = c + dc[i]
        if 0 <= nc < 100 and ladder[r][nc] == 1 and visited[r][nc] != x + 1:
            dfs(r, nc, x)
            return
    if 0 <= r + 1 < 100 and ladder[r + 1][c] == 1 and visited[r + 1][c] != x + 1:
        dfs(r + 1, c, x)
        return


for tc in range(1, 11):
    N = int(input())
    ladder = [list(map(int, input().split())) for _ in range(100)]
    visited = [[0] * 100 for _ in range(100)]
    ans = 0
    min_ladder = int(12e12)
    for y in range(100):
        total = 0
        if ladder[0][y] == 1:
            dfs(0, y, y)
            for v in visited:
                for cell in v:
                    if cell == y + 1:
                        total += 1
            if min_ladder >= total:
                min_ladder = total
                ans = y
    # for i in visited:
    #     print(*i)
    print(f'#{tc} {ans}')
