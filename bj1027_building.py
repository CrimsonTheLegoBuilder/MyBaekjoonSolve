import sys

n = int(sys.stdin.readline().strip())

arr = list(map(int, sys.stdin.readline().strip().split()))
dp = [0] * n

for i in range(n):
    inc = -1e10
    for j in range(i - 1, -1, -1):
        if (arr[j] - arr[i]) / (i - j) > inc:
            dp[i] += 1
            inc = (arr[j] - arr[i]) / (i - j)
    inc = -1e10
    for k in range(i + 1, n):
        if (arr[k] - arr[i]) / (k - i) > inc:
            dp[i] += 1
            inc = (arr[k] - arr[i]) / (k - i)

print(max(dp))
