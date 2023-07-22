import sys
import math


def cross(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    if len(d2) == 0:
        return
    return (d2[1] - d1[1]) * (d3[2] - d2[2]) - (d2[2] - d1[2]) * (d3[1] - d2[1])


def cross_ccw(a1, a2, a3, a4):  # CCW / for 4 dots
    v1 = [a2[1] - a1[1], a2[2] - a1[2]]
    v2 = [a4[1] - a3[1], a4[2] - a3[2]]
    ccw = v1[0] * v2[1] - v1[1] * v2[0]
    if ccw < 0:
        return 0
    else:
        return 1


def cal_length(a1, a2):
    l = (a1[1] - a2[1])**2 + (a1[2] - a2[2])**2
    return l


def in_triangle(arr):
    arr.sort(key=lambda d: d[2])  # sort on y
    miny, maxy = arr[0], arr[-1]
    hull_candidate = [miny, maxy]
    flag = 0
    length_of_v = ((miny[1] - maxy[1])**2 + (miny[2] - maxy[2])**2) ** 0.5
    distance_min = 1e12
    distance_max = -1e12
    max_r, max_l = [], []
    for i in arr[1:-1]:
        length = cross(maxy, miny, i) / length_of_v
        if cross(maxy, miny, i) == 0:
            flag += 1
        elif length > 0 and distance_max < length:
            if len(max_r):
                max_r.pop()
            max_r.append(i)
            distance_max = length
        elif length < 0 and distance_min > length:
            if len(max_l):
                max_l.pop()
            max_l.append(i)
            distance_min = length
    if flag == len(arr) - 2:
        return [miny, maxy]  # if all stars are standing in a straight line OR only two stars exists
    # elif len(max_l) == 0:
    #     for j in arr[1:-1]:
    #         if j == max_r[0]:
    #             hull_candidate.append(j)
    #         if cross(miny, max_r[0], j) < 0 or cross(maxy, max_r[0], j) > 0:
    #             hull_candidate.append(j)
    # elif len(max_r) == 0:
    #     for j in arr[1:-1]:
    #         if j == max_l[0]:
    #             hull_candidate.append(j)
    #         if cross(miny, max_l[0], j) > 0 or cross(maxy, max_l[0], j) < 0:
    #             hull_candidate.append(j)
    else:
        for j in arr[1:-1]:
            if j == max_l[0] or j == max_r[0]:
                hull_candidate.append(j)
            elif cross(miny, max_l[0], j) > 0 or cross(maxy, max_l[0], j) < 0 or cross(miny, max_r[0], j) < 0 or cross(maxy, max_r[0], j) > 0:
                hull_candidate.append(j)
    return hull_candidate


def sort_by_angle(arr):  # compose dots' array sorted by angle [[Θ1, x1, y1, vx1, vy1], [Θ2, x2, y2...]]
    d0 = arr[0]
    for i in range(1, len(arr)):
        th = math.atan2(arr[i][2] - d0[2], arr[i][1] - d0[1])
        arr[i][0] = th
    arr.sort(reverse=True)
    return arr


def fast_graham_scan(arr):  # get a convex hull of shadow
    hull_candidate = in_triangle(arr)
    if len(hull_candidate) == 2:
        return hull_candidate
    hull_candidate_angle = sort_by_angle(hull_candidate)
    d1 = hull_candidate_angle.pop()
    d2 = hull_candidate_angle.pop()
    hull = [d1, d2]
    while len(hull_candidate_angle) > 0:
        d3 = hull_candidate_angle.pop()
        if cross(d1, d2, d3) >= 0:
            hull.append(d3)
            d1, d2 = hull[-2], hull[-1]
        else:
            while cross(d1, d2, d3) < 0:
                hull.pop()
                d1, d2 = hull[-2], hull[-1]
            hull.append(d3)
            if len(hull_candidate_angle) == 0:
                break
            d1, d2 = hull[-2], hull[-1]
    return hull


def rotating_calipers(arr):
    a, b, c, d = 0, 1, 1, 2
    i, ei, fl2, efl2 = arr[a], arr[b], arr[c], arr[d]
    ans = []
    max_length = 0
    while True:
        if ei == arr[0]:
            break
        ccw = cross_ccw(i, ei, fl2, efl2)
        if ccw:
            if cal_length(i, fl2) > max_length:
                ans.clear()
                ans.append([i, fl2])
            elif cal_length(i, fl2) == max_length:
                ans.append([i, fl2])
            a += 1
            b += 1
        else:
            if cal_length(i, fl2) > max_length:
                ans.clear()
                ans.append([i, fl2])
            elif cal_length(i, fl2) == max_length:
                ans.append([i, fl2])
            c += 1
            d += 1
    return ans


n, t = map(int, sys.stdin.readline().strip().split())
stars = []
for _ in range(n):
    dx, dy, vx, vy = map(int, sys.stdin.readline().strip().split())
    stars.append([0.0, dx, dy, vx, vy])
hull_stars = fast_graham_scan(stars)
# print(hull_stars)
