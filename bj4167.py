import sys
from math import cos, sin, asin, radians


def arc_hav(p1, p2, l1, l2):
    h = hav(p2 - p1) + cos(radians(p1)) * cos(radians(p2)) * hav(l2 - l1)
    return asin(h**.5)


def hav(t1):
    return sin(radians(t1)/2)**2


r = 6371009
N = int(sys.stdin.readline().strip())
for _ in range(N):
    lat1, lon1, lat2, lon2 = map(float, sys.stdin.readline().strip().split())
    theta = 2 * arc_hav(lat1, lat2, lon1, lon2)
    arc = r * theta
    string = (2*r**2 - 2*r*r*cos(theta))**.5
    # print(arc, string)
    print(int(arc - string + .5))
