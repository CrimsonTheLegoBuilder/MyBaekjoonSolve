import sys

n = int(sys.stdin.readline().strip())
arr = []

for i in range(n):
    x, y, v = map(int, sys.stdin.readline().strip().split())
    d = (x ** 2 + y ** 2) ** 0.5
    t = d / v
    arr.append([t, i + 1])

arr.sort()

for j in arr:
    print(j[1])
