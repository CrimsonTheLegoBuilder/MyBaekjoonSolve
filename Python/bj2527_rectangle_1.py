import sys

for _ in range(4):
    x1, y1, x2, y2, x3, y3, x4, y4 = map(int, sys.stdin.readline().strip().split())
    if x1 > x4 or x2 < x3 or y1 > y4 or y2 < y3:
        sys.stdout.write('d\n')
    elif x1 == x4 or x2 == x3:
        if y1 == y4 or y2 == y3:
            sys.stdout.write('c\n')
        elif y1 <= y3 < y2 or y1 < y4 <= y2:
            sys.stdout.write('b\n')
    elif y1 == y4 or y2 == y3:
        sys.stdout.write('b\n')
    else:
        sys.stdout.write('a\n')
