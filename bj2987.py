import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def get_polygon_area(hull):  # get area
    pivot = (0, 0)
    len_hull = len(hull)
    p_area = 0
    for dh in range(len_hull):
        p_area += cross(pivot, hull[dh - 1], hull[dh - 1], hull[dh])
    return abs(p_area/2)


def inner_check(d, hull):  # return 1 if dot in polygon
    flag = 1
    for di in range(len(hull)):
        if cross(hull[di - 1], hull[di], hull[di], d) <= 0:
            flag = 0
    for di in range(len(hull)):
        if cross(hull[di - 1], d, d, hull[di]) == 0 and dot(hull[di - 1], d, d, hull[di]) >= 0:
            flag = 1
    return flag


triangle = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(3)]
n = int(sys.stdin.readline().strip())
area = get_polygon_area(triangle)
cnt = 0
for _ in range(n):
    x, y = map(int, sys.stdin.readline().strip().split())
    cnt += inner_check((x, y), triangle)
print(f'{area:.1f}')
print(cnt)
