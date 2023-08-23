import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


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
    try:
        x1, y1, x2, y2, x3, y3 = map(float, sys.stdin.readline().strip().split())
        x1_2, y1_2 = (x1 + x2)/2, (y1 + y2)/2
        x1_3, y1_3 = (x1 + x3)/2, (y1 + y3)/2
        vx2, vy2 = x2 - x1, y2 - y1
        vx3, vy3 = x3 - x1, y3 - y1
        x1_2_, y1_2_ = x1_2 + vy2, y1_2 - vx2
        x1_3_, y1_3_ = x1_3 + vy3, y1_3 - vx3
        centre = intersection_d((x1_2, y1_2), (x1_2_, y1_2_), (x1_3, y1_3), (x1_3_, y1_3_))
        # print(centre)
        r = cal_dist((x1, y1), centre)
        h = centre[0]
        k = centre[1]
        c = 2*h
        d = 2*k
        e = h*h + k*k - r*r
        p1, p2, p3, p4, p5 = '-', '-', '-', '-', '+'
        if h < 0:
            h = abs(h)
            p1 = '+'
        if k < 0:
            k = abs(k)
            p2 = '+'
        if c < 0:
            c = abs(c)
            p3 = '+'
        if d < 0:
            d = abs(d)
            p4 = '+'
        if e < 0:
            e = abs(e)
            p5 = '-'
        print(f'(x {p1} {h:.3f})^2 + (y {p2} {k:.3f})^2 = {r:.3f}^2')
        print(f'x^2 + y^2 {p3} {c:.3f}x {p4} {d:.3f}y {p5} {e:.3f} = 0\n')
    except:
        break
