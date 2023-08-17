import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


# def swap(d1, d2, i):
#     if d1[i] < d2[i]:
#         d1, d2 = d2, d1
#     return d1, d2


def inner_non_convex(d, hull):
    len_hull = len(hull)
    cnt = 0
    for di in range(len_hull):
        d1, d2 = hull[di], hull[(di+1) % len_hull]
        if d1[1] < d2[1]:
            d1, d2 = d2, d1
        # d1, d2 = swap(d1, d2, 1)
        if cross(d1, d, d, d2) == 0 and dot(d1, d, d, d2) >= 0:
            return 1
        if max(d1[0], d2[0]) < d[0]:
            continue
        if min(d1[1], d2[1]) > d[1]:
            continue
        if max(d1[1], d2[1]) <= d[1]:
            continue
        if cross(d1, d, d, d2) > 0:
            cnt += 1
    return cnt % 2


N = int(sys.stdin.readline().strip())
shield = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
Yeon = tuple(map(int, sys.stdin.readline().strip().split()))
Hun = tuple(map(int, sys.stdin.readline().strip().split()))
Jin = tuple(map(int, sys.stdin.readline().strip().split()))
# print(Yeon, Hun, Jin)
print(inner_non_convex(Yeon, shield))
print(inner_non_convex(Hun, shield))
print(inner_non_convex(Jin, shield))
