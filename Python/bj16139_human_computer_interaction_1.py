import sys

string = list(str(sys.stdin.readline().strip()))
n = int(sys.stdin.readline().strip())
dp = [0] * (len(string) + 1)
a, start, end = map(str, sys.stdin.readline().strip().split())

for i in range(len(string)):
    dp[i + 1] = dp[i]
    if string[i] == a:
        dp[i + 1] += 1

start, end = int(start), int(end)
ans = dp[end + 1] - dp[start]
print(ans)
for _ in range(n - 1):
    a, start, end = map(str, sys.stdin.readline().strip().split())
    start, end = int(start), int(end)
    ans = dp[end + 1] - dp[start]
    print(ans)
