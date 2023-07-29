import sys
from math import atan2, asin, pi


def cross_3(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def cross_4(d1, d2, d3, d4):  # cross product / CCW / for 4 dots
    v1 = [d2[0] - d1[0], d2[1] - d1[1]]
    v2 = [d4[0] - d3[0], d4[1] - d3[1]]
    return v1[0] * v2[1] - v1[1] * v2[0]


def dot_4(d1, d2, d3, d4):  # dot product / CCW / for 4 dots
    v1 = [d2[0] - d1[0], d2[1] - d1[1]]
    v2 = [d4[0] - d3[0], d4[1] - d3[1]]
    return v1[0] * v2[0] + v1[1] * v2[1]


def dist_square(a1, a2):  # get c^2 = a^2 + b^2
    return (a1[0] - a2[0])**2 + (a1[1] - a2[1])**2


def cal_angle_1(d1, d2, d3):  # calculate the angle between
    torque = abs(cross_3(d1, d2, d3))
    # arm = dist_square(d1, d2)**.5
    # f = dist_square(d2, d3)**.5
    arm = dot_4(d1, d2, d2, d3)
    angle = atan2(torque, -arm)
    if torque == 0 and arm < 0:
        return pi
    return angle


def cal_angle_2(d1, d2):  # calculate the angle between
    h = abs(d1[2] - d2[2])
    hypotenuse = dist_square(d1, d2)**.5
    angle = asin(h/hypotenuse)
    if d1[2] >= d2[2]:
        return angle
    else:
        return -angle


def fence(d1, d2):
    print(dist_square(d1, d2))
    print((d1[2] - d2[2])**2)
    print((dist_square(d1, d2) - (d1[2] - d2[2])**2)**.5)
    return (dist_square(d1, d2) - (d1[2] - d2[2])**2)**.5


def monotone_chain(arr):  # get hull
    arr.sort(key=lambda x: (x[0], x[1]))
    if len(arr) <= 2:
        return arr
    lower = []
    for dl in arr:
        while len(lower) > 1 and cross_3(lower[-2], lower[-1], dl) <= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(arr):
        while len(upper) > 1 and cross_3(upper[-2], upper[-1], du) <= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


t = int(sys.stdin.readline().strip())
for _ in range(t):
    n = int(sys.stdin.readline().strip())
    centre_and_r = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
    ans = 0
    hull_c = monotone_chain(centre_and_r)
    for i in range(n):
        ans += fence(hull_c[i % n], hull_c[i - 1])
        theta = (cal_angle_1(hull_c[i - 1], hull_c[i % n], hull_c[(i + 1) % n]) +
                 cal_angle_2(hull_c[i % n], hull_c[i - 1]) + cal_angle_2(hull_c[i % n], hull_c[(i + 1) % n]))
        print(cal_angle_1(hull_c[i - 1], hull_c[i % n], hull_c[(i + 1) % n]))
        print(cal_angle_2(hull_c[i % n], hull_c[i - 1]))
        print(cal_angle_2(hull_c[i % n], hull_c[(i + 1) % n]))
        print(theta)
        ans += hull_c[i][2] * theta
    print(ans)
