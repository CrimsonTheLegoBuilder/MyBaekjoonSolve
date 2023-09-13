import sys

x0, y0, z0, x1, y1, z1 = map(int, sys.stdin.readline().strip().split())
N = int(sys.stdin.readline().strip())
sticks = sorted(list(map(int, sys.stdin.readline().strip().split())))
length = ((x1-x0)**2 + (y1-y0)**2 + (z1-z0)**2)**.5
la, l1, l2 = sum(sticks), sum(sticks[:-1]), sticks[-1]
if la < length or length + l1 < l2:
    print("NO")
else:
    print("YES")
