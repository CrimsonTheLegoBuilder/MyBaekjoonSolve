import sys
from heapq import heappush, heappop

N = int(sys.stdin.readline().strip())
priority_q = []

for _ in range(N):
    x = int(sys.stdin.readline().strip())
    if x:
        heappush(priority_q, (abs(x), x))
    else:
        if priority_q:
            _, tmp = heappop(priority_q)
            print(tmp)
        else:
            print(0)
