import sys
from math import gcd
from functools import cmp_to_key
sys.setrecursionlimit(10000)


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def cal_dist_square(d1, d2):  # get c^2 = a^2 + b^2
    return (d1[0] - d2[0])**2 + (d1[1] - d2[1])**2


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


def comp(d1, d2):  # compare by slope
    return (d1[1] * d2[0] > d1[0] * d2[1]) - (d1[1] * d2[0] < d1[0] * d2[1])


def sort_by_angle(arr):  # compose dots' array sorted by slope [[x1, y1, vx1, vy1], [x2, y2...]]
    arr.sort(key=cmp_to_key(comp))
    return arr


def get_polygon_area(hull):  # get area
    pivot = (0, 0)
    a = len(hull)
    area = 0
    for i in range(a):
        area += cross(pivot, hull[i - 1], hull[i - 1], hull[i])
    return abs(area/2)


def monotone_chain(arr):  # get hull / sorting hull CCW
    arr.sort(key=lambda x: (x[0], x[1]))
    if len(arr) <= 2:
        return arr
    lower = []
    for dl in arr:
        while len(lower) > 1 and cross(lower[-2], lower[-1], lower[-1], dl) <= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(arr):
        while len(upper) > 1 and cross(upper[-2], upper[-1], upper[-1], du) <= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


def monotone_chain_r(arr):  # reversed / get hull / sorting hull CW
    arr.sort(key=lambda x: (-x[1], x[0]))
    if len(arr) <= 2:
        return arr
    lower = []
    for dl in arr:
        while len(lower) > 1 and cross(lower[-2], lower[-1], lower[-1], dl) >= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(arr):
        while len(upper) > 1 and cross(upper[-2], upper[-1], upper[-1], du) >= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


def rotating_calipers(hull):  # get largest distance of hull
    b = 1
    l = len(hull)
    max_dist = 0
    for a in range(l + 1):
        while (b + 1) % l != a % l and cross(hull[a % l], hull[(a + 1) % l], hull[b % l], hull[(b + 1) % l]) > 0:
            if max_dist < cal_dist_square(hull[a % l], hull[b % l]):
                max_dist = cal_dist_square(hull[a % l], hull[b % l])
            b += 1
        if max_dist < cal_dist_square(hull[a % l], hull[b % l]):
            max_dist = cal_dist_square(hull[a % l], hull[b % l])
    return max_dist


def inner_check(d, arr):  # return 0 if dot in polygon
    flag = 1
    for x in range(len(arr)):
        if cross(d, arr[x - 1], arr[x - 1], arr[x]) < 0:
            flag = 0
        if cross(d, arr[x - 1], arr[x - 1], arr[x]) == 0 and dot(arr[x - 1], d, d, arr[x]) < 0:
            flag = 0
    return flag


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


def intersection_dots(hull1, hull2):
    cross_dots = []
    for i in range(len(hull1)):
        for j in range(len(hull2)):
            if (hull2[j - 1] == hull1[i] or hull2[j - 1] == hull1[i - 1]) | \
                (cross(hull1[i - 1], hull1[i], hull1[i], hull2[j - 1]) == 0 and
                    sorted([hull1[i - 1], hull1[i], hull2[j - 1]], key=lambda o: (o[0], o[1]))[1] == hull2[j - 1]):
                d1 = hull2[j - 1]
                cross_dots.append(d1)
            if (hull1[i - 1] == hull2[j] or hull1[i - 1] == hull2[j - 1]) | \
                    (cross(hull2[j - 1], hull2[j], hull2[j], hull1[i - 1]) == 0 and
                     sorted([hull2[j - 1], hull2[j], hull1[i - 1]], key=lambda o: (o[0], o[1]))[1] == hull1[
                         i - 1]):
                d1 = hull1[i - 1]
                cross_dots.append(d1)
            if cross(hull1[i - 1], hull1[i], hull1[i], hull2[j - 1]) * cross(hull1[i], hull1[i - 1], hull1[i - 1], hull2[j]) > 0 and \
                    cross(hull2[j - 1], hull2[j], hull2[j], hull1[i - 1]) * cross(hull2[j], hull2[j - 1], hull2[j - 1], hull1[i]) > 0:
                # prevent ZeroDivisionError
                if hull1[i - 1][0] - hull1[i][0] == 0 and hull2[j - 1][1] - hull2[j][1] == 0:
                    x = hull1[i - 1][0]
                    y = hull2[j - 1][1]
                elif hull2[j - 1][0] - hull2[j][0] == 0 and hull1[i - 1][1] - hull1[i][1] == 0:
                    x = hull2[j - 1][0]
                    y = hull1[i - 1][1]
                elif hull1[i - 1][0] - hull1[i][0] == 0:
                    x = hull1[i - 1][0]
                    y = hull2[j - 1][1] + (hull2[j - 1][1] - hull2[j][1]) * (hull1[i - 1][0] - hull2[j - 1][0]) / (
                                hull2[j - 1][0] - hull2[j][0])
                elif hull1[i - 1][1] - hull1[i][1] == 0:
                    x = hull2[j - 1][0] + (hull2[j - 1][0] - hull2[j][0]) * (hull1[i - 1][1] - hull2[j - 1][1]) / (
                                hull2[j - 1][1] - hull2[j][1])
                    y = hull1[i - 1][1]
                elif hull2[j - 1][0] - hull2[j][0] == 0:
                    x = hull2[j - 1][0]
                    y = hull1[i - 1][1] + (hull1[i - 1][1] - hull1[i][1]) * (hull2[j - 1][0] - hull1[i - 1][0]) / (
                                hull1[i - 1][0] - hull1[i][0])
                elif hull2[j - 1][1] - hull2[j][1] == 0:
                    x = hull1[i - 1][0] + (hull1[i - 1][0] - hull1[i][0]) * (hull2[j - 1][1] - hull1[i - 1][1]) / (
                                hull1[i - 1][1] - hull1[i][1])
                    y = hull2[j - 1][1]
                else:
                    a = (hull1[i - 1][1] - hull1[i][1]) / (hull1[i - 1][0] - hull1[i][0])
                    b = (hull2[j - 1][1] - hull2[j][1]) / (hull2[j - 1][0] - hull2[j][0])
                    c = (hull1[i - 1][0] - hull1[i][0]) / (hull1[i - 1][1] - hull1[i][1])
                    d = (hull2[j - 1][0] - hull2[j][0]) / (hull2[j - 1][1] - hull2[j][1])

                    x = (hull2[j - 1][1] - hull1[i - 1][1] + a * hull1[i - 1][0] - b * hull2[j - 1][0]) / (a - b)
                    y = (hull2[j - 1][0] - hull1[i - 1][0] + c * hull1[i - 1][1] - d * hull2[j - 1][1]) / (c - d)
                cross_dots.append((x, y))
    cross_dots = list(set(cross_dots))
    return cross_dots


def cross_s(s1, s2):  # s_ = (No, x1, y1, x2, y2)
    result1 = (s1[3] - s1[1]) * (s2[2] - s1[4]) - (s1[4] - s1[2]) * (s2[1] - s1[3])
    result2 = (s1[1] - s1[3]) * (s2[4] - s1[2]) - (s1[2] - s1[4]) * (s2[3] - s1[1])
    result3 = (s2[3] - s2[1]) * (s1[2] - s2[4]) - (s2[4] - s2[2]) * (s1[1] - s2[3])
    result4 = (s2[1] - s2[3]) * (s1[4] - s2[2]) - (s2[2] - s2[4]) * (s1[3] - s2[1])
    flag1 = (result1 * result2 > 0) * (result3 * result4 > 0)
    if result1 == 0 and (s2[1] - s1[1]) * (s1[3] - s2[1]) + (s2[2] - s1[2]) * (s1[4] - s2[2]) >= 0:
        flag1 = 1
    if result2 == 0 and (s2[3] - s1[1]) * (s1[3] - s2[3]) + (s2[4] - s1[2]) * (s1[4] - s2[4]) >= 0:
        flag1 = 1
    if result3 == 0 and (s1[1] - s2[1]) * (s2[3] - s1[1]) + (s1[2] - s2[2]) * (s2[4] - s1[2]) >= 0:
        flag1 = 1
    if result4 == 0 and (s1[3] - s2[1]) * (s2[3] - s1[3]) + (s1[4] - s2[2]) * (s2[4] - s1[4]) >= 0:
        flag1 = 1
    return flag1


def cross_d(d, s):  # d = (x, y), s = (No, x1, y1, x2, y2)
    result1 = (s[3] - s[1]) * (d[1] - s[4]) - (s[4] - s[2]) * (d[0] - s[3])  # cross
    result2 = (d[0] - s[1]) * (s[3] - d[0]) + (d[1] - s[2]) * (s[4] - d[1])  # dot
    return (result1 == 0) * (result2 >= 0)


arr1 = []
# initialize:
for i in arr1:
    dic = {}
    value_i = i[2]
    for j in arr1:
        if i == j:
            continue
        else:
            x, y = j[0] - i[0], j[1] - i[1]
            gcd_ = gcd(abs(x), abs(y))
            temp = (x // gcd_, y // gcd_)

            if temp in dic:
                dic[temp] += j[2]
            else:
                dic[temp] = j[2]
