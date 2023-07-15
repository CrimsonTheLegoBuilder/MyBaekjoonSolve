import sys

n, w, h = map(int, sys.stdin.readline().strip().split())

d = (w ** 2 + h ** 2) ** 0.5

for _ in range(n):
    l = int(sys.stdin.readline())
    if l > d:
        print('NE')
    else:
        print('DA')
