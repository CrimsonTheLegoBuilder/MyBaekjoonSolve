import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


def get_polygon_area(hull):  # get area
    pivot = (0, 0)
    a = len(hull)
    area = 0
    for i in range(a):
        area += cross(pivot, hull[i - 1], hull[i - 1], hull[i])
    return abs(area/2)


def intersection_d(d1, d2, d3, d4):
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


for _ in range(int(sys.stdin.readline().strip())):
    x3, x5, y5, x7, y7 = map(float, sys.stdin.readline().strip().split())
    x2, y2 = x3/2, 0.0
    x4, y4 = (x3+x5)/2, y5/2
    x6, y6 = (x5+x7)/2, (y5+y7)/2
    x8, y8 = x7/2, y7/2
    D = ((0.0, 0.0), (x2, y2), (x3, 0.0), (x4, y4), (x5, y5), (x6, y6), (x7, y7), (x8, y8))
    d_1 = intersection_d(D[0], D[3], D[2], D[5])
    d_2 = intersection_d(D[2], D[5], D[4], D[7])
    d_3 = intersection_d(D[4], D[7], D[6], D[1])
    d_4 = intersection_d(D[6], D[1], D[0], D[3])
    A1 = get_polygon_area((D[0], D[2], d_1))
    A2 = get_polygon_area((D[2], D[4], d_2))
    A3 = get_polygon_area((D[4], D[6], d_3))
    A4 = get_polygon_area((D[6], D[0], d_4))
    A5 = get_polygon_area((d_1, d_2, d_3, d_4))
    R = int(cal_dist(d_1, d_2) + cal_dist(d_2, d_3) + cal_dist(d_3, d_4) + cal_dist(d_4, d_1))+1
    print(f'{A1:.3f} {A2:.3f} {A3:.3f} {A4:.3f} {A5:.3f} {R}')
