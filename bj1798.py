import sys
from math import cos, pi

while True:
    try:
        r, h, d1, a1, d2, a2 = map(float, sys.stdin.readline().strip().split())
        l = (r * r + h * h) ** .5
        a = abs(a1 - a2)
        if a > 180.0:
            a = 360.0 - a
        theta = abs(a) * r * pi / (180.0 * l)
        x = (d1 * d1 + d2 * d2 - 2 * d1 * d2 * cos(theta)) ** .5
        print('%.2f' % x)
    except Exception:
        break
