import sys
from math import pi


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def monotone_chain(arr):  # get hull / sorting hull CCW
    arr.sort(key=lambda x: (x[0], x[1]))
    if len(arr) <= 2:
        return arr
    lower = []
    for dl in arr:
        while len(lower) > 1 and cross(lower[-2], lower[-1], lower[-1], dl) <= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(arr):
        while len(upper) > 1 and cross(upper[-2], upper[-1], upper[-1], du) <= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


N, L = map(int, sys.stdin.readline().strip().split())
dots = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
hull = monotone_chain(dots)
R = 2*L*pi
R_ = sum([cal_dist(hull[i], hull[i-1]) for i in range(len(hull))])
print(int(R+R_+.5))
