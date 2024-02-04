import sys


def stack_(o):
    if o[0] == '1':
        stack.append(int(o[2:]))
    if o[0] == '2':
        if stack:
            print(stack.pop())
        else:
            print(-1)
    if o[0] == '3':
        print(len(stack))
    if o[0] == '4':
        if stack:
            print(0)
        else:
            print(1)
    if o[0] == '5':
        if stack:
            print(stack[-1])
        else:
            print(-1)


N = int(sys.stdin.readline().strip())
stack = []
for _ in range(N):
    order = sys.stdin.readline().strip()
    stack_(order)
