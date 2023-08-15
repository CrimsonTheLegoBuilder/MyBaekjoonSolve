import sys
from math import pi, sin, cos, tan


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


W, r, R = 85.0, 10.0, 6.0
for _ in range(int(sys.stdin.readline().strip())):
    l, degree = map(float, sys.stdin.readline().strip().split())
    T = l * 100.0
    theta = degree * pi / 180
    target = T, W / 2
    slope = tan(theta)
    xs = W / (2 * slope)
    step = W / slope
    # print()
    # print(target, theta, theta * 180.0 / pi, slope, xs, step)
    while xs + step < T:
        xs += step
        # print(xs)
    ds = xs - r * sin(theta), r * cos(theta)
    # print(xs, 0.0, *ds)
    d_ = ds[0] + 1.0, ds[1] + slope
    D = abs(cross(ds, d_, ds, target) * cos(theta) / 2)
    # print(ds, d_, D, D < R, cos(theta), cos(theta))
    if D > R:
        print('no')
    else:
        print('yes')
