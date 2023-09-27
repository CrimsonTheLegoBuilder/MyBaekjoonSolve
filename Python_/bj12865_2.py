# n, m = map(int, input().split())
# dp = [0] * (m+1)
# for _ in range(n):
#     w, v = map(int, input().split())
#     for j in range(m, w-1, -1):  # column index j = current weight
#         dp[j] = max(dp[j-w] + v, dp[j])
# print(dp[m])
# stuff = sorted([tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)])
# stuff = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
# for w, v in stuff:
#     for j in range(m, w-1, -1):  # column index j = current weight
#         dp[j] = max(dp[j-w] + v, dp[j])
# print(dp[m])
import sys
n, m = map(int, sys.stdin.readline().strip().split())
dp = [0] * (m+1)
stuff = sorted([tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)])
for w, v in stuff:
    for j in range(m, w-1, -1):  # column index j = current weight
        dp[j] = max(dp[j-w] + v, dp[j])
print(dp[m])
