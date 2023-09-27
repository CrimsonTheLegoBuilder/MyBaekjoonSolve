import sys
f = open("hexagon_no.txt", 'w')

hexagon_no = [i*(i*2-1) for i in range(708)]

N = int(sys.stdin.readline().strip())

dp = [1000001] * (N+1)
for i in range(1, N+1):
    for h in hexagon_no:
        if i == h:
            dp[i] = 1
        if i < h:
            break
        dp[i] = min(dp[i], dp[i-h]+1)

print(dp[N])
f.write(str(dp))
f.close()
