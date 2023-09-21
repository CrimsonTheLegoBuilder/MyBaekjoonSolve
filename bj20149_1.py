import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def is_cross(d1, d2, d3, d4):
    flag11 = cross(d1, d2, d2, d3)*cross(d2, d1, d1, d4)
    flag12 = cross(d3, d4, d4, d1)*cross(d4, d3, d3, d2)
    flag_1 = flag11 > 0 and flag12 > 0
    flag_2 = (cross(d1, d3, d3, d2) == 0 and dot(d1, d3, d3, d2) >= 0) |\
             (cross(d1, d4, d4, d2) == 0 and dot(d1, d4, d4, d2) >= 0) |\
             (cross(d3, d1, d1, d4) == 0 and dot(d3, d1, d1, d4) >= 0) |\
             (cross(d3, d2, d2, d4) == 0 and dot(d3, d2, d2, d4) >= 0)
    return flag_1 or flag_2


def intersection_d(d1, d2, d3, d4):  # l1 = (d1, d2), l2 = (d3, d4)
    if d1 == d2 or d1 == d4:
        return d1
    if d2 == d3 or d2 == d4:
        return d2
    if d1[0] - d2[0] == 0 and d3[1] - d4[1] == 0:
        x = d1[0]
        y = d3[1]
    elif d3[0] - d4[0] == 0 and d1[1] - d2[1] == 0:
        x = d3[0]
        y = d1[1]
    elif d1[0] - d2[0] == 0:
        x = d1[0]
        y = d3[1] + (d3[1] - d4[1]) * (d1[0] - d3[0]) / (d3[0] - d4[0])
    elif d1[1] - d2[1] == 0:
        x = d3[0] + (d3[0] - d4[0]) * (d1[1] - d3[1]) / (d3[1] - d4[1])
        y = d1[1]
    elif d3[0] - d4[0] == 0:
        x = d3[0]
        y = d1[1] + (d1[1] - d2[1]) * (d3[0] - d1[0]) / (d1[0] - d2[0])
    elif d3[1] - d4[1] == 0:
        x = d1[0] + (d1[0] - d2[0]) * (d3[1] - d1[1]) / (d1[1] - d2[1])
        y = d3[1]
    else:
        a = (d1[1] - d2[1]) / (d1[0] - d2[0])
        b = (d3[1] - d4[1]) / (d3[0] - d4[0])
        c = (d1[0] - d2[0]) / (d1[1] - d2[1])
        d = (d3[0] - d4[0]) / (d3[1] - d4[1])
        x = (d3[1] - d1[1] + a * d1[0] - b * d3[0]) / (a - b)
        y = (d3[0] - d1[0] + c * d1[1] - d * d3[1]) / (c - d)
    cross_dots = (x, y)
    return cross_dots


x1, y1, x2, y2 = map(int, sys.stdin.readline().strip().split())
d_1, d_2 = (x1, y1), (x2, y2)
if d_1 > d_2:
    d_1, d_2 = d_2, d_1
x3, y3, x4, y4 = map(int, sys.stdin.readline().strip().split())
d_3, d_4 = (x3, y3), (x4, y4)
if d_3 > d_4:
    d_3, d_4 = d_4, d_3
if is_cross(d_1, d_2, d_3, d_4):
    print(1)
    if (cross(d_1, d_2, d_3, d_4) == 0 and d_1 == d_4) or (cross(d_1, d_2, d_3, d_4) == 0 and d_2 == d_3) or \
            cross(d_1, d_2, d_3, d_4):
        print(*intersection_d(d_1, d_2, d_3, d_4))
    # elif cross(d_1, d_2, d_3, d_4) == 0:
    #     pass
else:
    print(0)
