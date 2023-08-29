import sys
sys.setrecursionlimit(10000)


drc = ((1, 0), (0, 1), (-1, 0), (0, -1))


def dfs(r, c, s):
    global ans
    cur = grid[r][c]
    visited[r][c] = 1
    for x in range(4):
        nr = r + drc[x][0]
        nc = c + drc[x][1]
        if 0 <= nr < N and 0 <= nc < N and cur - grid[nr][nc] == 1 and not visited[nr][nc]:
            dfs(nr, nc, s+1)
            return
    if ans < s:
        ans = s
        arr.clear()
        arr.append(cur)
    elif ans == s:
        arr.append(cur)


for tc in range(1, int(input())+1):
    N = int(input())
    grid = [tuple(map(int, input().split())) for _ in range(N)]
    ans = 0
    arr = []
    for i in range(N):
        for j in range(N):
            flag = 1
            flag2 = 0
            for y in range(4):
                ni = i + drc[y][0]
                nj = j + drc[y][1]
                if 0 <= ni < N and 0 <= nj < N and grid[ni][nj] - grid[i][j] == 1:  # 걸러내기 1
                    flag = 0
                if 0 <= ni < N and 0 <= nj < N and grid[ni][nj] - grid[i][j] == -1:  # 걸러내기 2
                    flag2 = 1
            if flag and flag2:
                visited = [[0]*N for _ in range(N)]
                dfs(i, j, 1)
    print(f'#{tc} {sorted(arr)[0]} {ans}')
