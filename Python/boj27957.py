import sys
from math import atan2, sin, cos, asin, pi, hypot
sys.setrecursionlimit(10000)
TOL = 1e-9


def norm(th):
    while th < 0:
        th += pi
    while th >= 2*pi:
        th -= 2*pi
    return th


def delta(c1, c2):
    x1, y1, r1 = c1
    x2, y2, r2 = c2
    return x1-x2, y1-y2, r1-r2


def dif(c1, c2):
    x1, y1, r1 = c1
    x2, y2, r2 = c2
    return abs(x1-x2) >= TOL or abs(y1-y2) >= TOL or abs(r1-r2) >= TOL


def h(c, th):
    x, y, r = c
    return sin(th)*x + cos(th)*y + r


def merge2disks(p, q, cur, nxt):
    if cur >= nxt:
        return []

    d = delta(p, q)
    dx, dy, dr = d
    if abs(dx) < TOL and abs(dy) < TOL:
        if dr >= 0:
            return [[cur, nxt, p]]
        else:
            return [[cur, nxt, q]]

    t = -dr/hypot(dx, dy)
    if t <= -1:
        return [[cur, nxt, p]]
    if t >= 1:
        return [[cur, nxt, q]]

    phi = atan2(dy, dx)
    x1 = asin(abs(t)) + (pi if t < 0 else 0)
    x2 = pi - x1
    x1 = norm(x1-phi)
    x2 = norm(x2-phi)
    if x1 > x2:
        x1, x2 = x2, x1

    # h2d = []
    if h(p, (x1+x2)/2) < h(q, (x1+x2)/2):
        h2d = [[0.0, x1, p], [x1, x2, q], [x2, 2*pi, p]]
    else:
        h2d = [[0.0, x1, q], [x1, x2, p], [x2, 2*pi, q]]

    hull = []
    for lo, hi, c in h2d:
        lo = max(lo, cur)
        hi = min(hi, nxt)
        if lo >= hi:
            continue
        hull.append([lo, hi, c])

    return hull


def merge(h1, h2):
    i, j = 0, 0
    cur = 0
    tmp = []
    while i < len(h1) and j < len(h2):
        if h1[i][1] < h2[j][1]:
            h2d = merge2disks(h1[i][2], h2[j][2], cur, h1[i][1])
            cur = h1[i][1]
            i += 1
            for x in h2d:
                tmp.append(x)
        else:
            h2d = merge2disks(h1[i][2], h2[j][2], cur, h2[j][1])
            cur = h2[j][1]
            j += 1
            for x in h2d:
                tmp.append(x)
    hull = []
    cur = 0
    for i in range(len(tmp)):
        if i + 1 == len(tmp) or dif(tmp[i + 1][2], tmp[i][2]):
            hull.append([cur, tmp[i][1], tmp[i][2]])
            cur = tmp[i][1]
    return hull


def solve(l, r, c):
    if l == r:
        return [[0, 2*pi, c[l]]]
    m = l + r >> 1
    hl = solve(l, m, c)
    hr = solve(m+1, r, c)
    return merge(hl, hr)


def hull_disks(c):
    for disk in c:
        disk[0], disk[1] = disk[1], disk[0]
    print(c)
    hull = solve(0, len(c)-1, c)
    # for _, _, a in hull:
    #     a[0], a[1] = a[1], a[0]
    return hull


def perimeter(hull):
    r = 0
    for i in range(len(hull)):
        p, q = hull[i-1], hull[i]
        d = delta(p[2], q[2])
        r += p[2][2] * (p[1] - p[0])
        r += ((d[0]**2 + d[1]**2) - d[2]**2) ** .5
        print(p[2][2] * (p[1] - p[0]), p[2][2], (p[1]-p[0]), ((d[0]**2 + d[1]**2) - d[2]**2) ** .5)
    return r


n = int(sys.stdin.readline().strip())
C = [list(map(float, sys.stdin.readline().strip().split())) for _ in range(n)]
H = hull_disks(C)
print(H)
R = perimeter(H)
print(R)
