import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def intersection_d(d1, d2, d3, d4):  # l1 = (d1, d2), l2 = (d3, d4)
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
if x1 > x2:
    d_1, d_2 = d_2, d_1
x3, y3, x4, y4 = map(int, sys.stdin.readline().strip().split())
d_3, d_4 = (x3, y3), (x4, y4)
if x3 > x4:
    d_3, d_4 = d_4, d_3
if x1 > x4 or x3 > x2:
    print(0)
elif not cross(d_1, d_2, d_3, d_4):
    if not cross(d_1, d_2, d_2, d_3) and d_2 == d_3:
        print(1)
        print(*d_2)
    elif not cross(d_1, d_2, d_2, d_3) and dot(d_1, d_3, d_3, d_2) >= 0:
        print(1)
    elif not cross(d_1, d_2, d_2, d_3) and d_1 == d_4:
        print(1)
        print(*d_1)
    elif not cross(d_1, d_2, d_2, d_3) and dot(d_2, d_4, d_4, d_1) >= 0:
        print(1)
    else:
        print(0)
elif cross(d_1, d_2, d_3, d_4):
    if (cross(d_1, d_2, d_2, d_3) * cross(d_2, d_1, d_1, d_4) > 0 and cross(d_3, d_4, d_4, d_1) * cross(d_4, d_3, d_3, d_2) > 0) or \
        (d_1 == d_3 or d_1 == d_4 or d_2 == d_3 or d_2 == d_4) or (not cross(d_1, d_3, d_3, d_2) or not cross(d_1, d_4, d_4, d_2)
         or not cross(d_3, d_1, d_1, d_4) or not cross(d_3, d_2, d_2, d_4)):
        x_, y_ = intersection_d(d_1, d_2, d_3, d_4)
        print(1)
        print(x_, y_)
    else:
        print(0)
