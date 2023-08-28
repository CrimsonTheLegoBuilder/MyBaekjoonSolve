import sys

limit = 121
qh = [0]*limit
for i in range(1, limit+1):
    qh[i-1] = (i**3+3*i**2+2*i)//6
N = int(sys.stdin.readline().strip())
dp = [300_001]*(N+1)
for x in range(1, N+1):
    for q in qh:
        if x == q:
            dp[x] = 1
        if x < q:
            break
        dp[x] = min(dp[x], dp[x-q] + 1)
print(dp)
