import sys


def cross_3(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def cross_4(a1, a2, a3, a4):  # cross product / CCW / line-segment intersection / for 4 dots
    v1 = [a2[0] - a1[0], a2[1] - a1[1]]
    v2 = [a4[0] - a3[0], a4[1] - a3[1]]
    return v1[0] * v2[1] - v1[1] * v2[0]


def cal_dist_square(a1, a2):  # get c^2 = a^2 + b^2
    return (a1[0] - a2[0])**2 + (a1[1] - a2[1])**2


def across_dots(hull1, hull2):
    cross_dots = []
    for i in range(len(hull1)):
        for j in range(len(hull2)):
            if cross_3(hull1[i - 1], hull1[i], hull2[j]) * cross_3(hull1[i], hull1[i - 1], hull2[j - 1]) > 0 and \
                    cross_3(hull2[j - 1], hull2[j], hull1[i]) * cross_3(hull2[j], hull2[j - 1], hull1[i - 1]) > 0:
                a = (hull1[i][1] - hull1[i - 1][1]) / (hull1[i][0] - hull1[i - 1][0])
                b = (hull2[i][1] - hull2[i - 1][1]) / (hull2[i][0] - hull2[i - 1][0])
                c = (hull1[i][0] - hull1[i - 1][0]) / (hull1[i][1] - hull1[i - 1][1])
                d = (hull2[i][0] - hull2[i - 1][0]) / (hull2[i][1] - hull2[i - 1][1])
                x = (a*hull1[i][0] - b*hull2[i][0] - hull1[i][1] + hull2[i][1]) / (a - b)
                y = (hull2[i][1] - hull1[i][1] - c*hull2[i][1] + d*hull1[i][1]) / (c - d)
                cross_dots.append((x, y))
    return cross_dots


def get_hull(arr1, arr2):
    hull_candidate = []
    len_1, len_2 = len(arr1), len(arr2)
    for i in arr1:
        flag = True
        for j in range(len_2):
            if cross_3(i, arr2[j], arr2[(j + 1) % len_2]) * cross_3(i, arr2[(j + 1) % len_2], arr2[(j + 2) % len_2]) < 0:
                flag = False
                break
        if flag:
            hull_candidate.append(i)
    for j in arr2:
        flag = True
        for i in range(len_1):
            if cross_3(j, arr1[i], arr1[(i + 1) % len_1]) * cross_3(j, arr1[(i + 1) % len_1], arr1[(i + 2) % len_1]) < 0:
                flag = False
                break
        if flag:
            hull_candidate.append(j)
    return hull_candidate


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


def rotating_calipers(hull):  # get max distance of hull
    b = 1
    l = len(hull)
    max_dist = 0
    for a in range(l + 1):
        while b + 1 != a and cross_4(hull[a % l], hull[(a + 1) % l], hull[b % l], hull[(b + 1) % l]) > 0:
            if max_dist < cal_dist_square(hull[a % l], hull[b % l]):
                max_dist = cal_dist_square(hull[a % l], hull[b % l])
            b += 1
        if max_dist < cal_dist_square(hull[a % l], hull[b % l]):
            max_dist = cal_dist_square(hull[a % l], hull[b % l])
    return max_dist


def get_polygon_area(hull):
    pivot = (0, 0)
    a = len(hull)
    area = 0
    for i in range(a):
        area += cross_3(pivot, hull[a - 1], hull[a])
    return area


n, m = map(int, sys.stdin.readline().strip().split())
arr_a = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
arr_b = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(m)]
hull_can = get_hull(arr_a, arr_b) + across_dots(arr_a, arr_b)
hull_intersection = monotone_chain(hull_can)
ans = get_polygon_area(hull_intersection)
print(ans)
