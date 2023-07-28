import sys
from itertools import *


def cross_3(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


n = int(sys.stdin.readline().strip())
arr = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]

# print(list(combinations(arr, 3)))
ans = 0
for i in list(combinations(arr, 3)):
    if abs(cross_3(i[0], i[1], i[2])/2) > ans:
        ans = abs(cross_3(i[0], i[1], i[2])/2)

print(ans)
