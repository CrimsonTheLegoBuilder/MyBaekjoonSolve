import sys

t = int(sys.stdin.readline())

for _ in range(t):
    x1, y1, x2, y2 = map(int, sys.stdin.readline().strip().split())
    cnt = 0
    n = int(sys.stdin.readline())
    for _ in range(n):
        cx, cy, r = map(int, sys.stdin.readline().strip().split())
        R1 = ((x1 - cx) ** 2 + (y1 - cy) ** 2) ** 0.5
        R2 = ((x2 - cx) ** 2 + (y2 - cy) ** 2) ** 0.5
        if ((R1 > r) & (R2 < r)) | ((R1 < r) & (R2 > r)):
            cnt += 1
        else:
            continue
    print(cnt)
