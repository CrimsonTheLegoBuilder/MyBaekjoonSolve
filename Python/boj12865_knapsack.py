n, m = map(int, input().split())
dp = [0] * (m+1)
for _ in range(n):
    w, v = map(int, input().split())
    for j in range(m, w-1, -1):  # column index j = current weight
        dp[j] = max(dp[j-w] + v, dp[j])
print(dp[m])
