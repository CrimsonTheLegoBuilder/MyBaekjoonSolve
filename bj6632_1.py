import sys
# sys.stdin = open('input_arable_area.txt', 'r')


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def non_convex_inner_check(hull_, d_):
    x_, y_ = d_
    cnt = 0
    for i_ in range(N):
        d_1, d_2 = hull_[i_-1], hull_[i_]
        if d_1[1] > d_2[1]:
            d_1, d_2 = d_2, d_1
        x1, y1 = d_1
        x2, y2 = d_2
        if cross(d_1, d_, d_, d_2) == 0 and dot(d_1, d_, d_, d_2) >= 0:
            return 1
        if x_ > max(x1, x2):
            continue
        if y_ <= min(y1, y2):
            continue
        if y_ > max(y1, y2):
            continue
        if cross(d_1, d_2, d_2, d_) < 0:
            continue
        if y_ == y1 == y2:
            continue
        if y1 < y_ <= y2:
            cnt += 1
        elif cross(d_1, d_2, d_2, d_) > 0:
            cnt += 1
    if cnt % 2:
        return 1
    else:
        return 0


def cross_s(hull_, r, c):
    d1 = r, c
    d2 = r+1, c+1
    d3 = r+1, c
    d4 = r, c+1
    n = len(hull_)
    for i_ in range(n):
        d_1, d_2 = hull_[i_-1], hull_[i_]
        if cross(d_1, d_2, d_2, d1) * cross(d_2, d_1, d_1, d2) > 0 and \
                cross(d1, d2, d2, d_1) * cross(d2, d1, d1, d_2) > 0:
            return 1
        if cross(d_1, d_2, d_2, d3) * cross(d_2, d_1, d_1, d4) > 0 and \
                cross(d3, d4, d4, d_1) * cross(d4, d3, d3, d_2) > 0:
            return 1
    return 0


while True:
    N = int(sys.stdin.readline().strip())
    if not N:
        break
    hull = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
    dots = {}
    for i in range(-100, 101):
        for j in range(-100, 101):
            if non_convex_inner_check(hull, (i, j)):
                dots.setdefault((i, j), 1)
    ans = 0
    square = []
    for i in range(-100, 101):
        for j in range(-100, 101):
            if dots.get((i, j)) and dots.get((i+1, j)) and dots.get((i, j+1)) and dots.get((i+1, j+1)):
                if not cross_s(hull, i, j) and non_convex_inner_check(hull, (i+0.5, j+0.5)):
                    ans += 1
                    square.append((i, j))
    print(ans)
    # print(square)


'''
13
-100 100
91 -100
-99 99
92 -99
-98 98
93 -98
-97 97
94 -97
-96 96
95 -96
-95 95
96 -95
-97 100

# 6
'''
