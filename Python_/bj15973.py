import sys


x1, y1, x2, y2 = map(int, sys.stdin.readline().strip().split())
x3, y3, x4, y4 = map(int, sys.stdin.readline().strip().split())
if x1 > x4 or x2 < x3 or y1 > y4 or y2 < y3:
    sys.stdout.write('NULL\n')
elif x1 == x4 or x2 == x3:
    if y1 == y4 or y2 == y3:
        sys.stdout.write('POINT\n')
    else:
        sys.stdout.write('LINE\n')
elif y1 == y4 or y2 == y3:
    sys.stdout.write('LINE\n')
else:
    sys.stdout.write('FACE\n')