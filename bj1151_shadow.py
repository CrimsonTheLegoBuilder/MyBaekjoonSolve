import sys
import math


def make_tree(x1, y1, z1, x2, y2, z2):
    arr = [[x1, y1, z1], [x2, y1, z1], [x1, y2, z1], [x1, y1, z2], [x1, y2, z2], [x2, y1, z2], [x2, y2, z1], [x2, y2, z2]]
    return arr


def make_vector(arr, d_l):
    vector = []
    for i in range(len(arr)):
        vector.append([arr[i][0] - d_l[0], arr[i][1] - d_l[1], arr[i][2] - d_l[2]])
    return vector


def l_to_p(v1, d_l):  # line from light end plane
    if v1[2] >= 0:
        return None
    if v1[0] == 0 and v1[1] == 0:
        x, y = d_l[0], d_l[1]
        return [x, y]
    elif v1[0] == 0:
        y = (d_l[2] * v1[1] / v1[2]) + d_l[1]
        x = d_l[0]
        return [x, y]
    elif v1[1] == 0:
        x = (d_l[2] * v1[0] / v1[2]) + d_l[0]
        y = d_l[1]
        return [x, y]
    else:
        x = -(d_l[2] * v1[0] / v1[2]) + d_l[0]
        y = -(d_l[2] * v1[1] / v1[2]) + d_l[1]
        return [x, y]


def con_sur(d):  # if tree's bottom side contact plane:
    if d[2] == 0 or d[5] == 0:
        area = abs((d[0] - d[3]) * (d[1] - d[4]))
        return area
    else:
        return 0


def sort_by_angle(arr, d):  # compose sorted dots' array by angle [[Θ, x, y]...]
    sorted_by_angle = []
    for [x, y] in arr:
        th = math.atan2(y - d[1], x - d[0])
        sorted_by_angle.append([th, x, y])
    sorted_by_angle.sort(reverse=True)
    return sorted_by_angle


def cross(d1, d2, d3):  # CCW
    return (d2[-2] - d1[-2]) * (d3[-1] - d2[-1]) - (d2[-1] - d1[-1]) * (d3[-2] - d2[-2])


def cal_r(d1, d2):
    r = (d1[-1] - d2[-1])**2 + (d1[-2] - d2[-2])**2
    return r


def graham_scan(arr1, d):
    sorted_arr = [i[:] for i in arr1]
    d1 = [0, *d]
    d2 = sorted_arr.pop()
    hull = [d1, d2]
    while len(sorted_arr) > 0:
        d3 = sorted_arr.pop()
        if cross(d1, d2, d3) >= 0:
            hull.append(d3)
            d1, d2 = hull[-2], hull[-1]
        else:
            while cross(d1, d2, d3) < 0:
                hull.pop()
                d1, d2 = hull[-2], hull[-1]
            hull.append(d3)
            if len(sorted_arr) == 0:
                break
            d1, d2 = hull[-2], hull[-1]
    return hull


def cal_area(arr):
    pivot = [0, 0]
    area = 0
    for i in range(len(arr)):
        area += cross(pivot, arr[i - 1], arr[i])
    return area


xa, ya, za, xb, yb, zb = map(int, sys.stdin.readline().strip().split())
light = list(map(int, sys.stdin.readline().strip().split()))
tree = make_tree(xa, ya, za, xb, yb, zb)
l_to_t = make_vector(tree, light)
shadow_coord = []
for k in l_to_t:
    dot = l_to_p(k, light)
    if dot is None:
        continue
    else:
        shadow_coord.append(dot)
# shadow_coord = [l_to_p(k, light) for k in l_to_t if l_to_p(k, light)]
# list of intersection coord` of a line and a plane
if ((xa == xb) & (ya == yb)) | ((za == zb) & (ya == yb)) | ((xa == xb) & (za == zb)) |\
        (xa == xb == light[0]) | (ya == yb == light[1]):
    print(0)
elif len(shadow_coord) <= 4:  # when shadow extends infinitely
    print(-1)
else:
    shadow_car_coord = list(set([tuple(i) for i in shadow_coord]))
    shadow_car_coord.sort(key=lambda x: x[1])
    shadow_cyl_coord = sort_by_angle(shadow_car_coord[1:], shadow_car_coord[0])
    hull_shadow = graham_scan(shadow_cyl_coord, shadow_car_coord[0])
    ans = cal_area(hull_shadow) - con_sur([xa, ya, za, xb, yb, zb])
    print(abs(ans/2))
