import sys


def cross(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def cross_ccw(a1, a2, a3, a4):  # cross product / CCW / for 4 dots
    v1 = [a2[0] - a1[0], a2[1] - a1[1]]
    v2 = [a4[0] - a3[0], a4[1] - a3[1]]
    return v1[0] * v2[1] - v1[1] * v2[0]


def monotone_chain(arr):  # get hull
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


def cal_dist_square(a1, a2):  # get c^2 = a^2 + b^2
    return (a1[0] - a2[0])**2 + (a1[1] - a2[1])**2


def rotating_calipers(hull):  # get max distance of hull
    b = 1
    l = len(hull)
    max_dist = 0
    for a in range(l + 1):
        while (b + 1) % l != a % l and cross_ccw(hull[a % l], hull[(a + 1) % l], hull[b % l], hull[(b + 1) % l]) > 0:
            if max_dist < cal_dist_square(hull[a % l], hull[b % l]):
                max_dist = cal_dist_square(hull[a % l], hull[b % l])
            b += 1
        if max_dist < cal_dist_square(hull[a % l], hull[b % l]):
            max_dist = cal_dist_square(hull[a % l], hull[b % l])
    return max_dist


def pos_at_day_x(tup, x):  # compose list of positions at day x
    pos_at_x_th = [(i[0] + i[2]*x, i[1] + i[3]*x) for i in tup]
    return pos_at_x_th


def ternary_search(dots, x):  # get range of days during which the maximum length of the hull becomes smaller
    start, end = 0, x
    while end - start >= 3:
        mid_l = (start*2 + end) // 3  # 1/3
        mid_r = (start + end*2) // 3  # 2/3

        pos_x_l = pos_at_day_x(dots, mid_l)
        hull_l = monotone_chain(pos_x_l)
        max_l = rotating_calipers(hull_l)

        pos_x_r = pos_at_day_x(dots, mid_r)
        hull_r = monotone_chain(pos_x_r)
        max_r = rotating_calipers(hull_r)

        if max_l <= max_r:
            end = mid_r
        else:
            start = mid_l
        # print(start, end)
        # print(max_l, max_r)
    min_length = int(32e15)
    min_day = 0
    for i in range(start, end + 1):
        pos_at_i = pos_at_day_x(dots, i)
        hull_i = monotone_chain(pos_at_i)
        max_i = rotating_calipers(hull_i)
        if min_length > max_i:
            min_length = max_i
            min_day = i
    return min_day, min_length


if __name__ == '__main__':
    n, t = map(int, sys.stdin.readline().strip().split())
    stars = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
    ans = ternary_search(stars, t)
    print(ans[0])
    print(ans[1])


'''
example

3 3
0 0 2 0
5 0 -1 0
3 -3 1 1

2
5


2 1
-4 -2 2 1
4 2 -2 -1

1
10


10 1000000
1 1 5 5
5 5 -1 1
2 3 2 3
4 3 5 6
6 5 4 3
3 3 -1 -1
6 9 -1 -2
7 8 -2 -4
0 0 10 0
-2 -3 -4 5

0
208


8 100
1 1 0 0
2 2 -1 0
3 4 1 0
5 4 -3 4
6 7 9 -1
0 3 3 3
2 0 -3 -3
1 0 2 1

0
74


10 100
50 60 -5 -5
50 50 -1 -1
20 30 -2 -3
40 30 -5 -6
-60 -50 4 3
-15 -30 1 1
60 90 -5 -6
70 80 -2 -4
0 0 7 0
-20 -30 4 5

10
8500


'''
