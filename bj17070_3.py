import sys

N = int(sys.stdin.readline().strip())
grid = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
dp = [[[0]*(N+1) for _ in range(N+1)] for _ in range(3)]
dp[0][1][2] = 1
for i in range(1, N+1):
    for j in range(3, N+1):
        if grid[i-1][j-1]:
            continue
        if not grid[i-1][j-2]:
            dp[0][i][j] += dp[0][i][j-1] + dp[1][i][j-1]
        if not grid[i-2][j-1]:
            dp[2][i][j] += dp[2][i-1][j] + dp[1][i-1][j]
        if not grid[i-2][j-1] and not grid[i-1][j-2] and not grid[i-2][j-2]:
            dp[1][i][j] += dp[0][i-1][j-1] + dp[1][i-1][j-1] + dp[2][i-1][j-1]

# for row in dp:
#     print(*row)
if grid[0][2] or grid[N-1][N-1]:
    print(0)
else:
    print(dp[0][N][N]+dp[1][N][N]+dp[2][N][N])

'''
16
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
'''
