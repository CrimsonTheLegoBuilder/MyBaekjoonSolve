import sys

N = int(sys.stdin.readline().strip())
limit = int(1e6+1)
dp = [0] * limit
dp[0], dp[1] = 1, 3
for i in range(2, N+1):
    dp[i] = (dp[i-1] * 2 + dp[i-2]) % 9901  # 1차원 dp 점화식
print(dp[N])

N = int(sys.stdin.readline().strip())
limit = int(1e6+1)
dp = [[0, 0] for _ in range(limit)]
dp[0][0], dp[0][1] = 1, 0  # 2차원 배열로 구할 수 있는 점화식.
dp[1][0], dp[1][1] = 1, 2  # mod 연산은 나누기에서 분배법칙이 통하지 않는다는 것을 잊어버렸다.
dp[2][0], dp[2][1] = 3, 4  # 1차원 배열로 구할 수 있는 식을 찾아야 했다.
for i in range(3, N+1):
    dp[i][0] = ((dp[i-1][0] // 3 + dp[i-1][1] // 2) * 3) % 9901
    dp[i][1] = (((dp[i-1][0] * 2) // 3 + dp[i-1][1] // 2) * 2) % 9901
# print(dp[:10])
print((dp[N][0] + dp[N][1]) % 9901)

