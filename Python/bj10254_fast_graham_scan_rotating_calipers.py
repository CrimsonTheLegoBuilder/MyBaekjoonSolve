import sys
import math
# WARNING!! WRONG CODE ####
# WARNING!! WRONG CODE ####
# WARNING!! WRONG CODE ####


def cross(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    if len(d2) == 0:
        return 0
    return (d2[1] - d1[1]) * (d3[2] - d2[2]) - (d2[2] - d1[2]) * (d3[1] - d2[1])


def cross_ccw(a1, a2, a3, a4):  # cross product / CCW / for 4 dots
    v1 = [a2[1] - a1[1], a2[2] - a1[2]]
    v2 = [a4[1] - a3[1], a4[2] - a3[2]]
    return v1[0] * v2[1] - v1[1] * v2[0]


def cal_dist_square(a1, a2):
    l = (a1[1] - a2[1])**2 + (a1[2] - a2[2])**2
    return l


def get_candidate(arr):
    arr.sort(key=lambda d: (d[2], d[1]))  # sort on y
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
        return [miny, maxy]  # if all dots are standing in a straight line OR only two dots exists
    for j in arr[1:-1]:
        out_l, out_r = False, False
        if len(max_r) != 0:
            if j == max_r[0] or cross(miny, max_r[0], j) < 0 or cross(maxy, max_r[0], j) > 0:
                out_r = True
        if len(max_l) != 0:
            if j == max_l[0] or cross(miny, max_l[0], j) > 0 or cross(maxy, max_l[0], j) < 0:
                out_l = True
        if out_r or out_l:
            hull_candidate.append(j)
    return hull_candidate


def sort_by_angle(arr):  # compose dots' array sorted by angle [[Θ1, x1, y1, vx1, vy1], [Θ2, x2, y2...]]
    d0 = arr[0]
    for i in range(1, len(arr)):
        th = math.atan2(arr[i][2] - d0[2], arr[i][1] - d0[1])
        arr[i][0] = th
    # arr.sort(key=lambda d: (-d[0], d[2]))
    arr.sort(reverse=True)
    return arr


def fast_graham_scan(arr):  # get a convex hull
    hull_candidate = get_candidate(arr)
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


def rotating_calipers(hull):
    b = 1
    l = len(hull)
    max_1 = hull[0]
    max_2 = hull[1]
    max_dist = 0
    for a in range(l + 1):
        while b + 1 != a and cross_ccw(hull[a % l], hull[(a + 1) % l], hull[b % l],
                                       hull[(b + 1) % l]) > 0:
            if max_dist < cal_dist_square(hull[a % l], hull[b % l]):
                max_dist = cal_dist_square(hull[a % l], hull[b % l])
                max_1 = hull[a % l]
                max_2 = hull[b % l]
            b += 1
        if max_dist < cal_dist_square(hull[a % l], hull[b % l]):
            max_dist = cal_dist_square(hull[a % l], hull[b % l])
            max_1 = hull[a % l]
            max_2 = hull[b % l]
    max_pair = [max_1, max_2]
    return max_dist
    # return max_pair


n = int(sys.stdin.readline().strip())
dots = []
for _ in range(n):
    dx, dy = map(int, sys.stdin.readline().strip().split())
    dots.append([0.0, dx, dy])
hull_dots = fast_graham_scan(dots)
# print(hull_cities)
if len(hull_dots) <= 2:
    highway = cal_dist_square(hull_dots[0], hull_dots[1])
else:
    highway = rotating_calipers(hull_dots)
print(highway)
