import sys

N = int(sys.stdin.readline().strip())
limit = int(1e6+1)
dp = [0] * limit
dp[0], dp[1] = 1, 3
for i in range(2, limit):
    dp[i] = (dp[i-1] * 2 + dp[i-2]) % 9901
print(dp[N])
# N = int(sys.stdin.readline().strip())
# limit = int(1e6+1)
# dp = [[0, 0] for _ in range(limit)]
# dp[0][0], dp[0][1] = 1, 0
# dp[1][0], dp[1][1] = 1, 2
# dp[2][0], dp[2][1] = 3, 4
# for i in range(3, N+1):
#     dp[i][0] = ((dp[i-1][0] // 3 + dp[i-1][1] // 2) * 3) % 9901
#     dp[i][1] = (((dp[i-1][0] * 2) // 3 + dp[i-1][1] // 2) * 2) % 9901
# # print(dp[:10])
# print((dp[N][0] + dp[N][1]) % 9901)

