import sys

n = int(sys.stdin.readline().strip())

for _ in range(n):
    x1, y1, r1, x2, y2, r2 = map(int, sys.stdin.readline().strip().split())
    dist = ((x1 - x2) ** 2 + (y1 - y2) ** 2) ** 0.5
    R = r1 + r2
    if (x1 == x2) & (y1 == y2):
        if r1 == r2:
            print(-1)
        else:
            print(0)
    elif ((x1 == x2) & (y1 == y2)) | (R < dist) | (max(r1, r2) > dist + min(r1, r2)):
        print(0)
    elif (R == dist) | (max(r1, r2) == dist + min(r1, r2)):
        print(1)
    else:
        print(2)
