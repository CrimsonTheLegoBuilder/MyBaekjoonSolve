import sys
import collections as co

n = int(input())
dq1 = co.deque()


def pop_fx(q):
    if len(q) == 0:
        return -1
    return q.popleft()


def pop_bx(q):
    if len(q) == 0:
        return -1
    return q.pop()


for _ in range(n):
    a = sys.stdin.readline().strip().split()
    if a[0] == 'push_front':
        dq1.appendleft(a[1])
    if a[0] == 'push_back':
        dq1.append(a[1])
    elif a[0] == 'pop_front':
        print(pop_fx(dq1))
    elif a[0] == 'pop_back':
        print(pop_bx(dq1))
    elif a[0] == 'size':
        print(len(dq1))
    elif a[0] == 'empty':
        if len(dq1) == 0:
            print(1)
        else:
            print(0)
    elif a[0] == 'front':
        if len(dq1) == 0:
            print(-1)
        else:
            print(dq1[0])
    elif a[0] == 'back':
        if len(dq1) == 0:
            print(-1)
        else:
            print(dq1[-1])
