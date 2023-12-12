import sys

n = int(sys.stdin.readline().strip())
grid = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
grid2 = []
for i in range(n):
    grid2.append(list(reversed(grid[i])))
dp = [[0]*(n+1) for _ in range(n+1)]
dp2 = [[0]*(n+1) for _ in range(n+1)]
for i in range(1, n+1):
    for j in range(1, n+1):
        dp[i][j] = dp[i][j-1] + dp[i-1][j] - dp[i-1][j-1] + grid[i-1][j-1]
for i in range(1, n+1):
    for j in range(1, n+1):
        dp2[i][j] = dp2[i][j-1] + dp2[i-1][j] - dp2[i-1][j-1] + grid2[i-1][j-1]

# cnt = 0
# for i in range(1, n+1):
#     for j in range(1, n+1):
#         for k in range(0, i):
#             for l in range(0, j):
#                 for m in range(i+1, n+1):
#                     for o in range(j+1, n+1):
#                         sum1 = dp[i][j] - dp[i][l] - dp[k][j] + dp[k][l]
#                         sum2 = dp[m][o] - dp[m][j] - dp[i][o] + dp[i][j]
#                         if sum1 == sum2:
#                             # print(sum1, i, j, k, l, m, o)
#                             cnt += 1
#                         sum1 = dp2[i][j] - dp2[i][l] - dp2[k][j] + dp2[k][l]
#                         sum2 = dp2[m][o] - dp2[m][j] - dp2[i][o] + dp2[i][j]
#                         if sum1 == sum2:
#                             # print(sum1, i, j, k, l, m, o)
#                             cnt += 1
cnt = 0
for i in range(1, n+1):
    for j in range(1, n+1):
        dic1 = {}
        dic2 = {}
        for k in range(0, i):
            for l in range(0, j):
                sum11 = dp[i][j] - dp[i][l] - dp[k][j] + dp[k][l]
                sum12 = dp2[i][j] - dp2[i][l] - dp2[k][j] + dp2[k][l]
                dic1.setdefault(sum11, 0)
                dic2.setdefault(sum12, 0)
                dic1[sum11] += 1
                dic2[sum12] += 1
        for m in range(i+1, n+1):
            for o in range(j+1, n+1):
                sum2 = dp[m][o] - dp[m][j] - dp[i][o] + dp[i][j]
                if dic1.get(sum2):
                    cnt += dic1[sum2]
                    # print(sum1, i, j, k, l, m, o)
                sum2 = dp2[m][o] - dp2[m][j] - dp2[i][o] + dp2[i][j]
                if dic2.get(sum2):
                    cnt += dic2[sum2]
                    # print(sum1, i, j, k, l, m, o)

print(cnt)
# print(dp[n][n])
# for row in dp:
#     print(*row)
