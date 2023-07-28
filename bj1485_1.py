import sys
from itertools import *


def cal_dist_square(a1, a2):  # get c^2 = a^2 + b^2
    return (a1[0] - a2[0])**2 + (a1[1] - a2[1])**2


n = int(sys.stdin.readline().strip())

for _ in range(n):
    a = set()
    square = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(4)]
    for (x, y) in list(combinations(square, 2)):
        a.add(cal_dist_square(x, y))
    if len(a) == 2:
        print(1)
    else:
        print(0)
