import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def get_polygon_area(hull):  # get area
    pivot = (0, 0)
    len_hull = len(hull)
    p_area = 0
    for dh in range(len_hull):
        p_area += cross(pivot, hull[dh - 1], hull[dh - 1], hull[dh])
    return p_area


def m(order):
    hull = []
    # hull = [(0, 0)]
    i, j = 0, 0
    for o in order:
        i += dij[D[o]][0]
        j += dij[D[o]][1]
        hull.append((i, j))
    return hull


D = {'N': 0, 'E': 1, 'W': 2, 'S': 3}
dij = ((0, 1), (1, 0), (-1, 0), (0, -1))
for _ in range(int(sys.stdin.readline().strip())):
    OD = str(sys.stdin.readline().strip())
    arr = m(OD)
    A = get_polygon_area(arr)
    # print(A)
    # ans = "CCW" if A > 0 else "CW"
    ans = "CCW"
    print(ans)
# def make_dots(order):
#     hull = [(0, 0)]
#     i, j = 0, 0
#     for o in order:
#         if o == 'N':
#             j += 1
#             hull.append((i, j))
#         if o == 'E':
#             i += 1
#             hull.append((i, j))
#         if o == 'W':
#             i -= 1
#             hull.append((i, j))
#         if o == 'S':
#             j -= 1
#             hull.append((i, j))
#     return hull
