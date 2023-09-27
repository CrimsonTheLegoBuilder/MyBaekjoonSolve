import sys
from collections import deque


N = int(sys.stdin.readline().strip())
temp = tuple(map(int, sys.stdin.readline().strip().split()))
deq = deque([(temp[i], i+1) for i in range(N)])
ans = []
for _ in range(N):
    s, a = deq.popleft()
    ans.append(a)
    deq.rotate(-s+(s > 0))
print(*ans)
