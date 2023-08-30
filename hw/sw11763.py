def dfs(x, cnt, n):
    global total
    # print(cnt)
    if cnt > total:
        return
    if x == N:
        total = min(total, cnt + grid[n][0])
        return
    for i in range(1, N):
        if n != i and not visited[i]:
            visited[i] = 1
            dfs(x+1, cnt + grid[n][i], i)
            visited[i] = 0


for tc in range(1, int(input())+1):
    N = int(input())
    grid = [tuple(map(int, input().split())) for _ in range(N)]
    visited = [0]*N
    total = int(12e12)
    dfs(1, 0, 0)
    print(f'#{tc} {total}')
