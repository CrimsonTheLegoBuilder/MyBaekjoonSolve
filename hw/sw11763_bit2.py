def dfs(x, cnt, n):
    global total, v
    # print(cnt)
    if cnt > total:
        return
    if x == N:
        total = min(total, cnt + grid[n][0])
        return
    for i in range(1, N):
        if n != i and not v & 1 << i:
            v |= 1 << i
            dfs(x+1, cnt + grid[n][i], i)
            v ^= 1 << i


for tc in range(1, int(input())+1):
    N = int(input())
    grid = [tuple(map(int, input().split())) for _ in range(N)]
    v = 1
    total = int(12e12)
    dfs(1, 0, 0)
    print(f'#{tc} {total}')
