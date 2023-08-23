import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def get_polygon_area(hull):  # get area
    pivot = (0, 0)
    len_hull = len(hull)
    p_area = 0
    for dh in range(len_hull):
        p_area += cross(pivot, hull[dh - 1], hull[dh - 1], hull[dh])
    return abs(p_area/2)


ans = []
while True:
    dots = tuple(map(float, sys.stdin.readline().strip().split()))
    if not dots[0]:
        break
    hull_ = [(dots[i], dots[i+1]) for i in range(1, int(dots[0])*2+1, 2)]
    # print(hull_)
    area = get_polygon_area(hull_)
    ans.append(area)

for i in ans:
    print(int(i+0.5))
