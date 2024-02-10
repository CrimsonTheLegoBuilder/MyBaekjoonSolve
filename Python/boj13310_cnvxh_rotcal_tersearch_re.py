import sys
from math import hypot
TOL = 1e-7


class Pos:
    def __init__(self, x_, y_, vx_, vy_):
        self.x = x_
        self.y = y_
        self.vx = vx_
        self.vy = vy_

    def __sub__(self, p):
        return Pos(self.x - p.x, self.y - p.y, 0, 0)

    def __floordiv__(self, p) -> int or float:
        return self.x * p.y - self.y * p.x

    def __pow__(self, p) -> int or float:
        return self.x * p.x + self.y * p.y

    # def __lt__(self, p):
    #     if self.x == p.x:
    #         return self.y < p.y
    #     return self.x < p.x
    #
    # def __le__(self, p):
    #     if self.x == p.x:
    #         return self.y <= p.y
    #     return self.x <= p.x

    def mag(self) -> float:
        return hypot(self.x, self.y)

    def euc(self) -> int or float:
        return self.x * self.x + self.y * self.y

    def move(self, d: int):
        return Pos(self.x + d * self.vx, self.y + d * self.vy, 0, 0)


def cross(d1: Pos, d2: Pos, d3: Pos) -> int or float:
    return (d2 - d1) // (d3 - d2)


def monotone_chain(candi: list) -> list:
    candi.sort(key=lambda p: (p.x, p.y))
    if len(candi) <= 2:
        return candi
    lower = []
    for dl in candi:
        while len(lower) > 1 and cross(lower[-2], lower[-1], dl) <= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(candi):
        while len(upper) > 1 and cross(upper[-2], upper[-1], du) <= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


def ccw(hull: list, i: int, f: int) -> int or float:
    sz = len(hull)
    i1, i2 = hull[i], hull[(i + 1) % sz]
    f1, f2 = hull[f % sz], hull[(f + 1) % sz]
    return (i2 - i1) // (f2 - f1)


def rotating_calipers(hull: list) -> int or float:
    f = 1
    sz = len(hull)
    max_dist = 0
    for i in range(sz):
        while ccw(hull, i, f) > 0:
            max_dist = max(max_dist, (hull[i] - hull[f]).euc())
            f = (f + 1) % sz
        max_dist = max(max_dist, (hull[i] - hull[f]).euc())
    return max_dist


def move(candi: list, d: int) -> list:
    return [p.move(d) for p in candi]


def ternary_search(candi: list, d: int) -> tuple:
    s, e = 0, d
    while e - s >= 3:
        mid_l = (s + s + e) // 3
        mid_r = (s + e + e) // 3
        ll = rotating_calipers(monotone_chain(move(candi, mid_l)))
        rr = rotating_calipers(monotone_chain(move(candi, mid_r)))
        if ll <= rr:
            e = mid_r
        else:
            s = mid_l
    min_length = int(1e17)
    min_day = 0
    for i in range(s, e + 1):
        max_l = rotating_calipers(monotone_chain(move(candi, i)))
        if min_length > max_l:
            min_length = max_l
            min_day = i
    return min_day, min_length


if __name__ == '__main__':
    N, T = map(int, sys.stdin.readline().strip().split())
    candidate = [Pos(*map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
    D, L = ternary_search(candidate, T)
    print(f'{D}\n{L}')


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
