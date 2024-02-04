import sys

x1, y1 = map(int, sys.stdin.readline().strip().split())
x2, y2 = map(int, sys.stdin.readline().strip().split())
x3, y3 = map(int, sys.stdin.readline().strip().split())

cross = (x2 - x1) * (y3 - y2) - (x3 - x2) * (y2 - y1)

print(2 * (cross > 0) - 1 if cross else 0)  # 3항연산자
