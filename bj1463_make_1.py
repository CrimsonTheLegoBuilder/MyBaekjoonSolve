import sys

arr = [0] * int(1e6 + 1)
arr[1], arr[2], arr[3] = 0, 1, 1

n = int(sys.stdin.readline().strip())

for i in range(4, n + 1):
    arr[i] = arr[i - 1] + 1
    if i % 2 == 0:
        arr[i] = min(arr[i // 2], arr[i - 1]) + 1
    if i % 3 == 0:
        arr[i] = min(arr[i // 3], arr[i - 1]) + 1
    if i % 6 == 0:
        arr[i] = (min(arr[i // 3], arr[i // 2], arr[i - 1])) + 1

print(arr[i])

arr2 = [0] * int(1e6 + 1)
arr2[1] = 0

for i in range(2, n + 1):
    arr2[i] = arr2[i - 1] + 1
    if i % 2 == 0:
        arr2[i] = min(arr2[i // 2] + 1, arr2[i])
    if i % 3 == 0:
        arr2[i] = min(arr2[i // 3] + 1, arr2[i])

# for i in range(n):
#     if arr[i] != arr2[i]:
#         print(i)

