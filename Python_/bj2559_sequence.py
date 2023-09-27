import sys

n, m = map(int, sys.stdin.readline().strip().split())
arr = list(map(int, sys.stdin.readline().strip().split()))
dp = [0] * (n - m + 1)
dp[0] = sum(arr[0:m])
ans = dp[0]

for i in range(1, len(dp)):
    dp[i] = dp[i - 1] + arr[i + m - 1] - arr[i - 1]
    if ans < dp[i]:
        ans = dp[i]

print(ans)
