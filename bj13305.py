import sys

n = int(sys.stdin.readline().strip())
dist = tuple(map(int, sys.stdin.readline().strip().split()))
price = tuple(map(int, sys.stdin.readline().strip().split()))
cur = price[0]
ans = 0

for i in range(n - 1):
    ans += dist[i] * cur
    if cur > price[i + 1]:
        cur = price[i + 1]
print(ans)
