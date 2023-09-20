def dfs(x, v):
    if x == N:
        return 0
    if dp[x][v] < limit:
        return dp[x][v]
    for i in range(N):
        if P[x][i] and not v & 1 << i:
            temp = dfs(x + 1, v | 1 << i) + P[x][i]
            dp[x][v] = min(temp, dp[x][v])
    return dp[x][v]


for tc in range(int(input())):
    N = int(input())
    P = [tuple(map(int, input().split())) for _ in range(N)]
    n_l = 0
    limit = int(12e12)
    dp = [[limit] * (1 << N) for _ in range(N)]
    ans = dfs(0, 0)
    print(f"#{tc + 1} {ans}")
