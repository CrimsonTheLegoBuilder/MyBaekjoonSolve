import sys


def cross(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def cross_ccw(a1, a2, a3, a4):  # cross product / CCW / for 4 dots
    v1 = [a2[0] - a1[0], a2[1] - a1[1]]
    v2 = [a4[0] - a3[0], a4[1] - a3[1]]
    return v1[0] * v2[1] - v1[1] * v2[0]


# def monotone_chain(arr):
#     arr.sort(key=lambda x: (x[0], x[1]))
#     if len(arr) <= 2:
#         return arr
#     lower = []
#     for dl in arr:
#         while len(lower) > 1 and cross(lower[-2], lower[-1], dl) <= 0:
#             lower.pop()
#         lower.append(dl)
#     upper = []
#     for du in reversed(arr):
#         while len(upper) > 1 and cross(upper[-2], upper[-1], du) <= 0:
#             upper.pop()
#         upper.append(du)
#     return lower[:-1] + upper[:-1]


def cal_dist_square(a1, a2):
    return (a1[0] - a2[0])**2 + (a1[1] - a2[1])**2


# def rotating_calipers(hull):
#     a, b = 0, 1
#     l = len(hull)
#     max_pair = [hull[a], hull[b]]
#     max_dist = 0
#     while True:
#         if a == l:
#             break
#         i, ei, fl2, efl2 = hull[a], hull[(a + 1) % l], hull[b % l], hull[(b + 1) % l]
#         ccw = cross_ccw(i, ei, fl2, efl2)
#         diagonal = cal_dist_square(i, fl2)
#         if max_dist < diagonal:
#             max_dist = diagonal
#             max_pair = [i, fl2]
#         if ccw < 0:
#             a += 1
#         else:
#             b += 1
#     return max_pair


for _ in range(int(sys.stdin.readline().strip())):
    n = int(sys.stdin.readline().strip())
    cities = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
    cities.sort(key=lambda x: (x[0], x[1]))
    if len(cities) <= 2:
        hull_cities = cities
    else:
        lower = []
        for dl in cities:
            while len(lower) > 1 and cross(lower[-2], lower[-1], dl) <= 0:
                lower.pop()
            lower.append(dl)
        upper = []
        for du in reversed(cities):
            while len(upper) > 1 and cross(upper[-2], upper[-1], du) <= 0:
                upper.pop()
            upper.append(du)
        hull_cities = lower[:-1] + upper[:-1]
    a, b = 0, 1
    l = len(hull_cities)
    city_1 = hull_cities[a]
    city_2 = hull_cities[b]
    max_dist = 0
    while True:
        if a == l:
            break
        i, ei, fl2, efl2 = hull_cities[a], hull_cities[(a + 1) % l], hull_cities[b % l], hull_cities[(b + 1) % l]
        ccw = cross_ccw(i, ei, fl2, efl2)
        diagonal = cal_dist_square(i, fl2)
        if max_dist < diagonal:
            max_dist = diagonal
            city_1 = hull_cities[a]
            city_2 = hull_cities[b % l]
        if ccw < 0:
            a += 1
        else:
            b += 1
    print(*city_1, *city_2)
