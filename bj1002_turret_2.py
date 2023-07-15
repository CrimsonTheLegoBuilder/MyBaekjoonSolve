import sys

n = int(sys.stdin.readline().strip())

for _ in range(n):
    x1, y1, r1, x2, y2, r2 = map(int, sys.stdin.readline().strip().split())
    dist = ((x1 - x2) ** 2 + (y1 - y2) ** 2) ** 0.5
    R = r1 + r2
    if (dist == 0) & (r1 == r2):
        if r1 == 0:
            print(1)
        else:
            print(-1)
    elif (dist == 0) | (R < dist) | (abs(r1 - r2) > dist):
        print(0)
    elif (R == dist) | (abs(r1 - r2) == dist):
        print(1)
    else:
        print(2)
