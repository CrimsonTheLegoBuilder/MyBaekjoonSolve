import sys
from math import atan2, asin, pi
from functools import cmp_to_key


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


def comp(c1, c2):  # abs-y coord - x-coord - r
    if c1[1]-c1[2] > c2[1]-c2[2]:
        return 1
    elif c1[1]-c1[2] < c2[1]-c2[2]:
        return -1
    elif c1[1]-c1[2] == c2[1]-c2[2]:
        if c1[0] > c2[0]:
            return 1
        elif c1[0] < c2[0]:
            return -1
        elif c1[0] == c2[0]:
            if c1[2] < c2[2]:
                return 1
            elif c1[2] > c2[2]:
                return -1
            elif c1[2] == c2[2]:
                return 0


def is_c_in_c(c1, c2):  # c_n = (cx, cy, cr)
    x1, y1, r1 = c1
    x2, y2, r2 = c2
    vx, vy = x2 - x1, y2 - y1
    c2c = (vx * vx + vy * vy) ** .5
    if r1 >= c2c + r2 or r2 >= c2c + r1:
        return 1
    else:
        return 0


def get_slope(c1, c2):  # c_n = (cx, cy, cr)
    x1, y1, r1 = c1
    x2, y2, r2 = c2
    vx, vy = x2 - x1, y2 - y1
    c2c = (vx * vx + vy * vy) ** .5
    th = atan2(vy, vx)
    if th < 0:
        th += 2*pi
    p = asin((r2-r1)/c2c)
    th -= p
    if abs(th) < 0.00000001:
        th = 0.0
    elif th >= 2*pi:
        th -= 2*pi
    elif th < 0:
        th += 2*pi
    return th


def cal_hull_r(hull):
    r_h = 0.0
    lh = len(hull)
    for i in range(lh):
        c1, r, sp = hull[i-1]
        c2, _, sc = hull[i]
        t = sc - sp
        if t < 0:
            t += 2*pi
        r_h += r * t
        x1, y1, r1 = circles[c1]
        x2, y2, r2 = circles[c2]
        r_h += (cal_dist((x1, y1), (x2, y2))**2 - (r1-r2)**2)**.5
    return r_h


def dfs():
    hull = []
    min_s = 3*pi
    nxt = None
    p = 0
    for w in range(1, n):
        if not is_c_in_c(circles[p], circles[w]):
            tmp_s = get_slope(circles[p], circles[w])
            if min_s > tmp_s:
                min_s = tmp_s
                nxt = (w, circles[w][2], tmp_s)
    if nxt is None:
        return circles[0][2] * pi * 2
    hull.append(nxt)
    last = (0, nxt[0])
    while 1:
        min_s = 3 * pi
        # print(nxt)
        # print(last)
        p, s_p = nxt[0], nxt[2]
        for w in range(n):
            if w == p:
                continue
            if not is_c_in_c(circles[p], circles[w]):
                s_c = get_slope(circles[p], circles[w])
                tmp_s = s_c - s_p
                if tmp_s < 0:
                    tmp_s += 2*pi
                if min_s > tmp_s:
                    min_s = tmp_s
                    nxt = (w, circles[w][2], s_c)
        # print(nxt, (p, nxt[0]), last)
        if (p, nxt[0]) == last:
            # print(hull)
            return cal_hull_r(hull)
        hull.append(nxt)


for tc in range(int(sys.stdin.readline().strip())):
    n = int(sys.stdin.readline().strip())
    circles = sorted([tuple(map(float, sys.stdin.readline().strip().split())) for _ in range(n)], key=cmp_to_key(comp))
    # print(tc, circles)
    print(f"{dfs():.12f}")