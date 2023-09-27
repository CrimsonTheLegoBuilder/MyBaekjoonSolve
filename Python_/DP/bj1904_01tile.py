import sys

n = int(sys.stdin.readline().strip())
dp = [0] * int(1e6 + 1)
dp[1] = 1
dp[2] = 2
for i in range(3, n + 1):
    dp[i] = (dp[i - 1] + dp[i - 2]) % 15746

print(dp[n])

# import sys
fd = []


def fibo_dyna(n):
    if n == 1:
        return 1  # 인덱스 에러 방지
    fd = [0 for _ in range(n + 1)]
    fd[1] = 1
    fd[2] = 2
    for j in range(3, n + 1):
        fd[j] = (fd[j - 1] + fd[j - 2]) % 15746
    return fd[n]


N = int(sys.stdin.readline().strip())
print(fibo_dyna(N))
