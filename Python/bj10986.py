import sys


def combi_2(x):
    return (x * (x - 1)) // 2


n, m = map(int, sys.stdin.readline().strip().split())
arr = list(map(int, sys.stdin.readline().strip().split()))
dp = [0] * (n + 1)
ans = 0

for i in range(1, n + 1):
    dp[i] = (dp[i - 1] + arr[i - 1]) % m

dic_dp = {}
for j in dp:
    if j in dic_dp:
        dic_dp[j] += 1
    else:
        dic_dp[j] = 1

for k in dic_dp.values():
    ans += combi_2(k)

print(ans)
