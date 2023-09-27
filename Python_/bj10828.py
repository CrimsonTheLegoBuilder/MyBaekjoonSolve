import sys

n = int(input())
stack = []

arr = []
for _ in range(n):
    a = sys.stdin.readline().strip().split()
    arr.append(a)

for i in range(len(arr)):
    # print(arr)
    if len(arr[i]) == 2:
        stack.append(arr[i][1])
    else:
        if arr[i][0] == 'pop':
            if len(stack) != 0:
                p = stack.pop()
                print(p)
            else:
                print(-1)
        elif arr[i][0] == 'size':
            print(len(stack))
        elif arr[i][0] == 'empty':
            if len(stack) == 0:
                print(1)
            else:
                print(0)
        elif arr[i][0] == 'top':
            if len(stack) == 0:
                print(-1)
            else:
                print(stack[-1])
