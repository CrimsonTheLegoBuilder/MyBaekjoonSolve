import sys

dp = [[0] * 14 for _ in range(15)]
for i in range(14):
    dp[0][i] = i + 1

for i in range(1, 15):
    for j in range(14):
        if j == 0:
            dp[i][j] = 1
        else:
            dp[i][j] = dp[i][j-1] + dp[i-1][j]

n = int(sys.stdin.readline().strip())
for _ in range(n):
    a = int(sys.stdin.readline().strip())
    b = int(sys.stdin.readline().strip())
    print(dp[a][b-1])
