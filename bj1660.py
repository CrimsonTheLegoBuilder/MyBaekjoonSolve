import sys

limit = 121
dp = [0]*limit
for i in range(1, limit+1):
    dp[i-1] = (i**3+3*i**2+2*i)//6
N = int(sys.stdin.readline().strip())
ans = 300_001
for x in range(limit):
    N_ = N
    temp = 0
    for y in range(x, -1, -1):
        temp += N_ // dp[y]
        N_ %= dp[y]
    print(temp)
    ans = min(ans, temp)
print(ans)
print(dp)
