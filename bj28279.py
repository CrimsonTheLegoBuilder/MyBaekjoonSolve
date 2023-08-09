import sys
from collections import deque


def deque_deque():
    a = str(sys.stdin.readline().strip())
    if a[0] == '1':
        arr.appendleft(a.split()[1])
    if a[0] == '2':
        arr.append(a.split()[1])
    if a[0] == '3':
        if arr:
            print(arr.popleft())
        else:
            print(-1)
    if a[0] == '4':
        if arr:
            print(arr.pop())
        else:
            print(-1)
    if a[0] == '5':
        print(len(arr))
    if a[0] == '6':
        if arr:
            print(0)
        else:
            print(1)
    if a[0] == '7':
        if arr:
            print(arr[0])
        else:
            print(-1)
    if a[0] == '8':
        if arr:
            print(arr[-1])
        else:
            print(-1)


arr = deque()
n = int(sys.stdin.readline().strip())
for _ in range(n):
    deque_deque()
