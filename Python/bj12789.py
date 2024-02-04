import sys
from collections import deque

N = int(sys.stdin.readline().strip())
arr = list(map(int, sys.stdin.readline().strip().split()))
stack = []
cnt = 1
cur = 0
while cur < N:
    while stack:
        if stack[-1] != cnt:
            break
        if stack and stack[-1] == cnt:
            cnt += 1
            stack.pop()
    stack.append(arr[cur])
    cur += 1
# print(stack)
# print(cnt)

while stack:
    if stack[-1] != cnt:
        print('Sad')
        exit(0)
    stack.pop()
    cnt += 1
print('Nice')
