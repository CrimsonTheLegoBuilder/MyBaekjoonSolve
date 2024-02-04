import sys
from math import pi, cos


def cal_length_square(d1, d2):
    return (d2[0] - d1[0])**2 + (d2[1] - d1[1])**2


x1, y1, r1, x2, y2, r2 = map(float, sys.stdin.readline().strip().split())
