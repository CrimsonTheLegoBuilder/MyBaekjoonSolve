import sys


def cross_3(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def cross_4(a1, a2, a3, a4):  # cross product / CCW / for 4 dots
    v1 = [a2[0] - a1[0], a2[1] - a1[1]]
    v2 = [a4[0] - a3[0], a4[1] - a3[1]]
    return v1[0] * v2[1] - v1[1] * v2[0]


def cal_dist_square(a1, a2):  # get c^2 = a^2 + b^2
    return (a1[0] - a2[0])**2 + (a1[1] - a2[1])**2


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
    i_fl2 = []
    max_dist = 0
    for a in range(l + 1):
        while (b + 1) % l != a % l and cross_4(hull[a % l], hull[(a + 1) % l], hull[b % l], hull[(b + 1) % l]) > 0:
            if max_dist < cal_dist_square(hull[a % l], hull[b % l]):
                max_dist = cal_dist_square(hull[a % l], hull[b % l])
                i_fl2.clear()
                i_fl2.append((hull[a % l], hull[b % l]))
            b += 1
        if max_dist < cal_dist_square(hull[a % l], hull[b % l]):
            max_dist = cal_dist_square(hull[a % l], hull[b % l])
            i_fl2.clear()
            i_fl2.append((hull[a % l], hull[b % l]))
    return max_dist, i_fl2


n = int(sys.stdin.readline().strip())
arr1 = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
hull1 = monotone_chain(arr1)
dist = rotating_calipers(hull1)
print(dist)
