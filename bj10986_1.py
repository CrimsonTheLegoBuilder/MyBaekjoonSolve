import sys


def combi_2(x):
    return (x * (x - 1)) // 2


n, m = map(int, sys.stdin.readline().strip().split())
arr = list(map(int, sys.stdin.readline().strip().split()))
dp = [0] * (n + 1)
cs_mod = [0] * (n + 1)
ans = 0

for i in range(1, n + 1):
    dp[i] = (dp[i - 1] + arr[i - 1]) % m

for j in dp:
    cs_mod[j] += 1

for k in cs_mod:
    if k > 0:
        ans += combi_2(k)

print(ans)
