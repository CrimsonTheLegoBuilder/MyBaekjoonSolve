def tsp(v, now):
    if v == (1 << N) - 1:
        if towns[now][0]:
            return towns[now][0]
        else:
            return limit
    if dp[v][now] != limit:
        return dp[v][now]
    for i in range(1, N):
        if i != now and towns[now][i] and not v & 1 << i:
            temp = tsp(v | 1 << i, i) + towns[now][i]
            dp[v][now] = min(temp, dp[v][now])
    return dp[v][now]


for tc in range(1, int(input())+1):
    N = int(input())
    towns = [tuple(map(int, input().split())) for _ in range(N)]
    limit = int(12e12)
    dp = [[limit]*N for _ in range(1 << N)]
    ans = tsp(1, 0)
    print(f'#{tc} {ans}')
    # print(dp)
