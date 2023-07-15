import sys

n, m = map(int, sys.stdin.readline().strip().split())
arr = [[0, 0]]
for _ in range(n):
    arr.append(list(map(int, sys.stdin.readline().strip().split())))

dp = [[0] * (m + 1) for _ in range(n + 1)]

for i in range(1, n + 1):
    for j in range(1, m + 1):
        w, v = arr[i]
        if w <= j:
            dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - w] + v)
        else:
            dp[i][j] = dp[i - 1][j]

print(dp[-1][-1])
