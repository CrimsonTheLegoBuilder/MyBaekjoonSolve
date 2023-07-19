import sys

string = list(str(sys.stdin.readline().strip()))
n = int(sys.stdin.readline().strip())
dp = [[0] * 26 for _ in range(len(string) + 2)]

for i in range(len(string)):
    dp[i + 1][:] = dp[i][:]
    dp[i + 1][ord(string[i]) - 97] += 1

for _ in range(n):
    a, start, end = map(str, sys.stdin.readline().strip().split())
    start, end = int(start), int(end)
    ans = dp[end + 1][ord(a) - 97] - dp[start][ord(a) - 97]
    print(ans)
