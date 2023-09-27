import sys

n = int(sys.stdin.readline().strip())
arr = [0] * 500
dp = [0] * n
dp[0] = 1

for _ in range(n):
    a, b = map(int, sys.stdin.readline().strip().split())
    arr[a] = b

arr = [i for i in arr if i != 0]
print(arr)

for i in range(n - 1):
    for j in range(i + 1):
        if arr[j] < arr[i + 1]:
            if dp[i + 1] < dp[j]:
                dp[i + 1] = dp[j]
    dp[i + 1] += 1

print(max(dp))
