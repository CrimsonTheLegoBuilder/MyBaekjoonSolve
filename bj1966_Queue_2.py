import collections as cl
import sys

for _ in range(int(input())):
    _, m = map(int, sys.stdin.readline().strip().split())
    q = cl.deque(int(i) for i in sys.stdin.readline().strip().split())
    cnt = 0
    t = m

    while True:
        Mx = max(q)
        if q[0] == Mx:
            q.popleft()
            cnt += 1
            if t != 0:
                t -= 1
            else:
                print(cnt)
                break
        else:
            q.rotate(-1)
            if t != 0:
                t -= 1
            else:
                t = len(q)-1
