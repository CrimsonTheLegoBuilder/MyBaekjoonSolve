import sys

n = int(sys.stdin.readline().strip())
arr = sorted(list(map(int, sys.stdin.readline().strip().split())))
target = int(sys.stdin.readline().strip())
start, end = 0, n-1
cnt = 0
# print(arr)
while start < end:
    if arr[start] + arr[end] == target:
        cnt += 1
        # print(arr[start], arr[end])
        start += 1
        end -= 1
    elif arr[start] + arr[end] < target:
        start += 1
    else:
        end -= 1
print(cnt)
