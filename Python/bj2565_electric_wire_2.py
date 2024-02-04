import sys

n = int(sys.stdin.readline().strip())
dic = {}
arr = []
ans = []
dp = [0] * n
dp[0] = 1

for _ in range(n):
    a, b = map(int, sys.stdin.readline().strip().split())
    dic[a] = b
    arr.append(a)

arr.sort()
for k in arr:
    ans.append(dic[k])

for i in range(n - 1):
    for j in range(i + 1):
        if ans[j] < ans[i + 1]:
            if dp[i + 1] < dp[j]:
                dp[i + 1] = dp[j]
    dp[i + 1] += 1

print(max(dp))
