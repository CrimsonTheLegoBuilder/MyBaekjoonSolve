import sys


def cross(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def cross_ccw(a1, a2, a3, a4):  # cross product / CCW / for 4 dots
    v1 = [a2[0] - a1[0], a2[1] - a1[1]]
    v2 = [a4[0] - a3[0], a4[1] - a3[1]]
    return v1[0] * v2[1] - v1[1] * v2[0]


def monotone_chain(arr):
    arr.sort(key=lambda x: (x[0], x[1]))
    if len(arr) <= 2:
        return arr
    lower = []
    for dl in arr:
        while len(lower) > 1 and cross(lower[-2], lower[-1], dl) <= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(arr):
        while len(upper) > 1 and cross(upper[-2], upper[-1], du) <= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


def cal_dist_square(a1, a2):
    return (a1[0] - a2[0])**2 + (a1[1] - a2[1])**2


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
    return max_pair


for _ in range(int(sys.stdin.readline().strip())):
    n = int(sys.stdin.readline().strip())
    cities = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
    cities.sort(key=lambda x: (x[0], x[1]))
    if len(cities) <= 2:
        highway = cities
    else:
        hull_cities = monotone_chain(cities)
        highway = rotating_calipers(hull_cities)
    city_1, city_2 = highway[0], highway[1]
    print(*city_1, *city_2)
