import collections as cl
import sys

input = sys.stdin.readline

t = int(input())

for _ in range(t):
    n, m = map(int, input().split())
    q = cl.deque(map(int, input().strip().split()))
    N = cl.deque(range(1, n+1))
    M = N[m]
    cnt = 0

    while q:
        N.rotate(-1 * q.index(max(q)))
        temp = N.popleft()
        q.rotate(-1 * q.index(max(q)))
        q.popleft()
        cnt += 1

        if temp == M:
            print(cnt)
            break
