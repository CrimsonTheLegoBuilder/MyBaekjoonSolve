import sys

n, m = map(int, sys.stdin.readline().strip().split())
dp = [[0] * (m + 1) for _ in range(n + 1)]
for i in range(1, n + 1):
    w, v = map(int, sys.stdin.readline().strip().split())
    for j in range(1, m + 1):  # column index j = current weight
        if w <= j:
            dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - w] + v)
        else:
            dp[i][j] = dp[i - 1][j]
print(dp[n][m])
