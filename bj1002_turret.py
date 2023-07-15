import sys

n = int(sys.stdin.readline().strip())

for _ in range(n):
    x1, y1, r1, x2, y2, r2 = map(int, sys.stdin.readline().strip().split())
    if x1 == x2 and y1 == y2:
        if r1 == r2:
            print(-1)
        else:
            print(0)
    else:
        r = ((x1 - x2) ** 2 + (y1 - y2) ** 2) ** (1/2)
        if abs(r1 - r2) < r < r1 + r2:
            print(2)
        elif r1 + r2 == r or abs(r1 - r2) == r:
            print(1)
        else:
            print(0)
