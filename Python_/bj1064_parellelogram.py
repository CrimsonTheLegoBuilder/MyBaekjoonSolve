import sys

xa, ya, xb, yb, xc, yc = map(int, sys.stdin.readline().strip().split())
v1 = [xb - xa, yb - ya]
v2 = [xc - xb, yc - yb]
v3 = [xa - xc, ya - yc]
arr = [(v1[0] ** 2 + v1[1] ** 2) ** 0.5, (v2[0] ** 2 + v2[1] ** 2) ** 0.5, (v3[0] ** 2 + v3[1] ** 2) ** 0.5]
arr.sort()
if not v1[0] * v2[1] - v1[1] * v2[0]:
    sys.stdout.write('-1.0\n')
else:
    ans = (arr[2] - arr[0]) * 2
    sys.stdout.write(f'{ans}\n')
