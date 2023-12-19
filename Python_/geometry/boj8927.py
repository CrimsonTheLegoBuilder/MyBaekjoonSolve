import sys


class Pos:
    def __init__(self, x_, y_):
        self.x = x_
        self.y = y_

    def dist(self, d1):
        return (self.x - d1.x)**2 + (self.y - d1.y)**2

    def cross(self, d1):
        ret = (self.x * d1.y) - (self.y * d1.x)
        return ret


def cross(d0, d1, d2):
    ret = (d1.x - d0.x) * (d2.y - d1.y) - (d1.y - d0.y) * (d2.x - d1.x)
    return ret


def monotone_chain():
    D.sort(key=lambda pos: (pos.x, pos.y))
    if len(D) <= 2:
        return D
    uh, dh = [], []
    for d in D:
        while len(dh) > 1 and cross(dh[-2], dh[-1], d) <= 0:
            dh.pop()
        dh.append(d)
    for d in reversed(D):
        while len(uh) > 1 and cross(uh[-2], uh[-1], d) <= 0:
            uh.pop()
        uh.append(d)
    return dh[:-1] + uh[:-1]


def vec(i_):
    f = (i_ + 1) % len(hull)
    i_ %= len(hull)
    x_ = hull[f].x - hull[i_].x
    y_ = hull[f].y - hull[i_].y
    return Pos(x_, y_)


def rotating_calipers():
    maximum = 0
    f = 0
    sz = len(hull)
    for i_ in range(sz):
        while vec(i_).cross(vec(f)) > 0:
            maximum = max(maximum, Pos.dist(hull[i_], hull[f]))
            f = (f+1) % sz
        maximum = max(maximum, Pos.dist(hull[i_], hull[f]))
    return maximum


for _ in range(int(sys.stdin.readline().strip())):
    D = []
    for _ in range(int(sys.stdin.readline().strip())):
        x, y, w = map(int, sys.stdin.readline().strip().split())
        D += [Pos(x, y), Pos(x+w, y), Pos(x, y+w), Pos(x+w, y+w)]
    hull = monotone_chain()
    ans = rotating_calipers()
    print(ans)
