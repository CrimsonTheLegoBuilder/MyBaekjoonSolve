import sys

n = int(sys.stdin.readline().strip())
arr = list(map(int, sys.stdin.readline().strip().split()))

for i in range(1, n):
    arr[i] = max(arr[i], arr[i] + arr[i - 1])

ans = max(arr)
print(ans)
