import sys

a = str(sys.stdin.readline().strip())
b = str(sys.stdin.readline().strip())
dp = [0] * len(b)

for i in a:
    cnt = 0
    for j in range(len(b)):
        if cnt < dp[j]:
            cnt = dp[j]
        elif b[j] == i:
            dp[j] = cnt + 1

print(max(dp))
