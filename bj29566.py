import sys
from math import acos, sin, cos, isclose

x1, y1, x2, y2, x3, y3 = map(int, sys.stdin.readline().strip().split())
r1, r2, r3 = map(int, sys.stdin.readline().strip().split())

a_ = r1**2
b_ = r2**2
c_ = (x1-x2)**2 + (y1-y2)**2
cos_t = (a_+c_-b_)/(2*r1*c_**.5)
if cos_t > 1.0:
    print('Impossible')
else:
    theta = acos(cos_t)
    # print(theta)
    vx, vy = x2-x1, y2-y1
    vx1, vy1 = vx*cos(theta) - vy*sin(theta), vx*sin(theta) + vy*cos(theta)
    vx2, vy2 = vx*cos(-theta) - vy*sin(-theta), vx*sin(-theta) + vy*cos(-theta)
    # print(vx1, vy1)
    # print(vx2, vy2)
    x_1, y_1 = x1 + vx1*(a_/c_)**.5, y1 + vy1*(a_/c_)**.5
    x_2, y_2 = x1 + vx2*(a_/c_)**.5, y1 + vy2*(a_/c_)**.5
    # print(x_1, y_1)
    # print(x_2, y_2)
    if isclose((x3-x_1)**2+(y3-y_1)**2, float(r3**2)):
        print(f"{x_1:.6f} {y_1:.6f}")
    elif isclose((x3-x_2)**2+(y3-y_2)**2, float(r3**2)):
        print(f"{x_2:.6f} {y_2:.6f}")
    else:
        print('Impossible')
