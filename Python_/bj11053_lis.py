import sys

n = int(sys.stdin.readline().strip())

arr = list(map(int, sys.stdin.readline().strip().split()))
dp = [0] * n
dp[0] = 1

for i in range(n - 1):
    for j in range(i + 1):
        if arr[j] < arr[i + 1]:
            if dp[i + 1] < dp[j]:
                dp[i + 1] = dp[j]
    dp[i + 1] += 1

print(max(dp))
