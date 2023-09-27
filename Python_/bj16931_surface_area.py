import sys

n, m = map(int, sys.stdin.readline().strip().split())
arr = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
ans = 2 * n * m

for i in range(n):
    temp = 0
    for j in range(m - 1):
        temp += abs(arr[i][j] - arr[i][j + 1])
    ans += arr[i][0] + arr[i][-1] + temp

for k in range(m):
    temp = 0
    for l in range(n - 1):
        temp += abs(arr[l][k] - arr[l + 1][k])
    ans += arr[0][k] + arr[-1][k] + temp

sys.stdout.write(f'{ans}\n')
