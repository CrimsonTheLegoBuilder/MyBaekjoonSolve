import sys

n, k = map(int, sys.stdin.readline().split())
coins = [int(sys.stdin.readline().strip()) for i in range(n)]
ans = 0
for i in reversed(coins):
    if i > k:
        continue
    else:
        ans += k // i
        k %= i
print(ans)
