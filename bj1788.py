import sys

N = int(sys.stdin.readline().strip())
limit = int(1e9)
dp = [0] * (int(1e6) + 1)
dp[1] = 1
for i in range(2, (int(1e6) + 1)):
    dp[i] = (dp[i-1] + dp[i-2]) % limit
flag = 1
if not N:
    flag = 0
elif N < 0 and not N % 2:
    flag = -1
print(flag)
print(dp[abs(N)])


# print(fibo(abs(N)))
# def fibo(n):
#     if dp[n] > -1:
#         return dp[n]
#     dp[n] = (fibo(n-1) + fibo(n-2))  # % limit
#     return dp[n]
