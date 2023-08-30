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


while True:
    N = int(sys.stdin.readline().strip())
    if not N:
        break
    hull = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
    a = min(hull)
    b = max(hull)
    print(a, b)
    grid = [[1]*201 for _ in range(201)]
    for i in range(a[0]+100, b[0]+101):
        for j in range(201):
            grid[i][j] = 0
    ccw = get_polygon_area(hull)
    print(ccw)
    if ccw < 0:
        hull.reverse()
    ccw = get_polygon_area(hull)
    print(ccw)
    for i in range(N):
        d_1 = hull[i]
        x1, y1 = d_1
        d_2 = hull[(i+1) % N]
        x2, y2 = d_2
        if x1 == x2:
            continue
        if x1 < x2:
            for x in range(x1, x2+1):
                for y in range(-100, 100):
                    if cross(d_1, d_2, d_2, (x, y)) >= 0:
                        break
                    grid[x+100][y+100] = 1
        if x1 > x2:
            for x in range(x2, x1+1):
                for y in range(100, -100, -1):
                    if cross(d_1, d_2, d_2, (x, y)) >= 0:
                        break
                    grid[x+100][y+100] = 1

    for row in grid[a[0]+100:b[0]+101]:
        print(*row)
