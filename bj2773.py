import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def rotate_90(d1, d2):
    vy = d2[1] - d1[1]
    vx = d2[0] - d1[0]
    return d1[0] - vy, d1[1] + vx


def rotate_270(d1, d2):
    vy = d2[1] - d1[1]
    vx = d2[0] - d1[0]
    return d1[0] + vy, d1[1] - vx


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


N = int(sys.stdin.readline().strip())
for _ in range(N):
    xa, ya = tuple(map(float, sys.stdin.readline().strip().split()))
    xb, yb = tuple(map(float, sys.stdin.readline().strip().split()))
    xc, yc = tuple(map(float, sys.stdin.readline().strip().split()))
    A = (xa, ya)
    B = (xb, yb)
    C = (xc, yc)
    if cross(A, B, B, C) < 0:
        B, C = C, B
    x_ab, y_ab = rotate_90(A, B)
    x_ba, y_ba = rotate_270(B, A)
    x_ac, y_ac = rotate_270(A, C)
    x_bc, y_bc = rotate_90(B, C)
    A_ = (x_ab + x_ac)/2, (y_ab + y_ac)/2
    B_ = (x_ba + x_bc)/2, (y_ba + y_bc)/2
    X, Y = intersection_d(A, A_, B, B_)
    print(f'{X:.4f} {Y:.4f}')
