import sys

n = int(sys.stdin.readline().strip())
arr = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]

for i in range(1, n):
    arr[i][0] = arr[i][0] + min(arr[i - 1][1], arr[i - 1][2])
    arr[i][1] = arr[i][1] + min(arr[i - 1][0], arr[i - 1][2])
    arr[i][2] = arr[i][2] + min(arr[i - 1][0], arr[i - 1][1])

print(min(arr[n - 1]))
