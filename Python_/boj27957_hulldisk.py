import sys
from math import atan2, tan, cos, sin, pi
sys.setrecursionlimit(10000)


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


def convex_hull_of_disks(disks):
    if len(disks) == 1:
        return
    if len(disks) == 2:
        return
    mid = len(disks)//2
    left = disks[:mid]
    right = disks[mid:]

    ch1 = convex_hull_of_disks(left)
    ch2 = convex_hull_of_disks(right)
    return hull_merge(ch1, ch2)


def hull_merge(ch1, ch2):
    return 0


def hull2disks(disks):
    if len(disks) == 1:
        x, y, r = disks[0]
        return [(r, )]
    return 0


N = int(sys.stdin.readline().strip())
disks_ = sorted([tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)])
