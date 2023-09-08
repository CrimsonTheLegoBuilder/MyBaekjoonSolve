import sys

N, M = map(int, sys.stdin.readline().strip().split())

arr = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
dp = [[0] * (M+1) for _ in range(N+1)]

for r in range(1, N+1):
    for c in range(1, M+1):
        dp[r][c] = arr[r-1][c-1] + dp[r][c-1] + dp[r-1][c] - dp[r-1][c-1]


K = int(sys.stdin.readline().strip())
for _ in range(K):
    i, j, x, y = map(int, sys.stdin.readline().strip().split())
    print(dp[x][y] - dp[x][j-1] - dp[i-1][y] + dp[i-1][j-1])
