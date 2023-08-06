import sys
import math
from functools import cmp_to_key


def comp(x, y):
    return (x[1] * y[0] < y[1] * x[0]) - (x[1] * y[0] > y[1] * x[0])


def sort_by_angle(arr):  # compose dots' array sorted by angle [[Θ1, x1, y1, vx1, vy1], [Θ2, x2, y2...]]
    arr.sort(key=cmp_to_key(comp))
    return arr


a = [[1, 2], [1, 1], [-1, 1], [-1, 2], [-2, 1]]
print(sort_by_angle(a))
