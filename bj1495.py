import sys

N, S, M = map(int, sys.stdin.readline().strip().split())
vol = tuple(map(int, sys.stdin.readline().strip().split()))
dp = [[-1] * (M+1) for _ in range(N+1)]
dp[0][S] = S
for i in range(N):
    for j in range(M+1):
        if 0 <= j + vol[i] <= M and dp[i][j+vol[i]] > -1:
            dp[i+1][j] = j
        if 0 <= j - vol[i] <= M and dp[i][j-vol[i]] > -1:
            dp[i+1][j] = j
ans = -1
for j in range(M, -1, -1):
    if dp[-1][j] > -1:
        ans = j
        break
print(ans)
# for row in dp:
#     print(*row)
