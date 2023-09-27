import sys

n = int(sys.stdin.readline().strip())

arr = [0] * 101
arr[1] = 9
arr[2] = 17

for i in range(2, n):
    arr[i + 1] = ((arr[i] - 2) * 2 + 2)

print(arr[n] % int(1e9))
