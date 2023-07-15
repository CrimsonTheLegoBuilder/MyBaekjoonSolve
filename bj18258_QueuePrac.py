import sys
import collections as co

n = int(input())
q1 = co.deque()


def pop_x(q):
    if len(q) == 0:
        return -1
    return q.popleft()


for _ in range(n):
    a = sys.stdin.readline().strip().split()
    if a[0] == 'push':
        q1.append(a[1])
    elif a[0] == 'pop':
        print(pop_x(q1))
    elif a[0] == 'size':
        print(len(q1))
    elif a[0] == 'empty':
        if len(q1) == 0:
            print(1)
        else:
            print(0)
    elif a[0] == 'front':
        if len(q1) == 0:
            print(-1)
        else:
            print(q1[0])
    elif a[0] == 'back':
        if len(q1) == 0:
            print(-1)
        else:
            print(q1[-1])
