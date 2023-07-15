import sys

n = int(sys.stdin.readline().strip())

arr = list(map(int, sys.stdin.readline().strip().split()))
dp = [0] * n

for i in range(n - 1):
    inc = -1e10
    for k in range(i + 1, n):
        if (arr[k] - arr[i]) / (k - i) > inc:
            dp[i] += 1
            dp[k] += 1
            inc = (arr[k] - arr[i]) / (k - i)

print(max(dp))
