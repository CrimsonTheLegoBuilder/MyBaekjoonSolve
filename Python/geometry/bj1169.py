import sys

_ = int(sys.stdin.readline())
arr = tuple(map(int, sys.stdin.readline().strip().split()))
m = 0
stack = 0
l = 0
ans = [1]

for i in range(len(arr)):
    if m < arr[i] * 2:
        ans.append(i + 1)
        m = arr[i]
        stack = 0
        l = 0
    else:
        if stack == 0:
            stack += arr[i] * 2
            l = arr[i]
        elif arr[i] >= l:
            stack += arr[i]
            l = arr[i]
            if stack > m:
                ans.append(i + 1)
                m = arr[i]
                stack = 0
                l = 0
        else:
            pass

print(*ans)
