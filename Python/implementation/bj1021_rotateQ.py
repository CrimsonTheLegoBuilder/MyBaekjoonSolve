import sys
import collections as cl

n, _ = map(int, sys.stdin.readline().strip().split())
dq1 = cl.deque(int(i) for i in range(1, n+1))
arr = list(map(int, sys.stdin.readline().strip().split()))
cnt = 0

for j in arr:
    if dq1.index(j) <= (len(dq1) / 2):
        cnt += dq1.index(j)
        dq1.rotate(-dq1.index(j))
        dq1.popleft()
    else:
        x = len(dq1) - dq1.index(j)
        cnt += x
        dq1.rotate(x)
        dq1.popleft()

print(cnt)
