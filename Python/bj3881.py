import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


def dist(v):  # get c = (a^2 + b^2)^.5
    return (v[0]**2 + v[1]**2)**.5


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


while True:
    x1, y1, x2, y2, x3, y3 = map(int, sys.stdin.readline().strip().split())
    if x1 == y1 == x2 == y2 == x3 == y3 == 0:
        break
    v12 = x2 - x1, y2 - y1
    v13 = x3 - x1, y3 - y1
    v23 = x3 - x2, y3 - y2
    v21 = x1 - x2, y1 - y2
    l12 = dist(v12)
    l13 = dist(v13)
    l23 = dist(v23)
    l21 = dist(v21)
    v1_ = v12[0] + v13[0]*(l12/l13), v12[1] + v13[1]*(l12/l13)
    v2_ = v23[0] + v21[0]*(l23/l21), v23[1] + v21[1]*(l23/l21)
    d_1 = x1, y1
    d_2 = x1 + v1_[0], y1 + v1_[1]
    d_3 = x2, y2
    d_4 = x2 + v2_[0], y2 + v2_[1]
    # print(d_1, d_2, d_3, d_4)
    in_center = intersection_d(d_1, d_2, d_3, d_4)
    # print(in_center)
    a = cal_dist((x2, y2), (x3, y3))
    b = cal_dist((x1, y1), (x3, y3))
    c = cal_dist((x2, y2), (x1, y1))
    r = abs(cross((x2, y2), (x3, y3), (x3, y3), in_center)) / a
    d = cal_dist((x1, y1), in_center)
    e = cal_dist((x2, y2), in_center)
    f = cal_dist((x3, y3), in_center)
    s = (a+b+c)/2
    r1 = r * (s-r+d-e-f) / (2 * (s-a))
    r2 = r * (s-r-d+e-f) / (2 * (s-b))
    r3 = r * (s-r-d-e+f) / (2 * (s-c))
    print(f'{r1:.6f} {r2:.6f} {r3:.6f}')
