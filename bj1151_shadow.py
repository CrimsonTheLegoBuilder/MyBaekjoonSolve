import sys
import math
import collections as cl


def cross(d1, d2, d3):  # CCW
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def l_to_p(v1, d_l):  # line from light end plane
    if v1[2] >= 0:
        return None
    if not v1[0] and not v1[1]:
        x, y = d_l[0], d_l[1]
        return x, y
    elif v1[0] == 0:
        y = (d_l[2] * v1[1] / v1[2]) + d_l[1]
        x = d_l[0]
        return [x, y]
    elif v1[1] == 0:
        x = (d_l[2] * v1[0] / v1[2]) + d_l[0]
        y = d_l[1]
        return [x, y]
    else:
        x = (d_l[2] * v1[0] / v1[2]) + d_l[0]
        y = (d_l[2] * v1[1] / v1[2]) + d_l[1]
        return [x, y]


def con_sur(d):  # if tree's bottom side contact plane:
    if d[2] == 0 or d[5] == 0:
        area = abs((d[0] - d[3]) * (d[1] - d[4]))
        return area
    else:
        return 0


def sort_by_angle(arr, d):  # compose sorted dots' array by angle [[Θ, x, y]...]
    sorted_by_angle = []
    for [x, y] in arr:
        th = math.atan2(y - d[1], x - d[0])
        sorted_by_angle.append([th, x, y])
    sorted_by_angle.sort(reverse=True)
    return sorted_by_angle


def graham_scan(sorted_arr):
    while True:
        sorted_arr
