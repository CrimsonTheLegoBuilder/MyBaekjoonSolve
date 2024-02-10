import sys
from math import hypot
TOL = 1e-7


class Pos:
    def __init__(self, x_, y_):
        self.x = x_
        self.y = y_

    def __sub__(self, p):
        return Pos(self.x - p.x, self.y - p.y)

    def __floordiv__(self, p) -> int or float:
        return self.x * p.y - self.y * p.x

    def __pow__(self, p) -> int or float:
        return self.x * p.x + self.y * p.y

    def __lt__(self, p):
        if self.x == p.x:
            return self.y < p.y
        return self.x < p.x

    def mag(self) -> float:
        return hypot(self.x, self.y)


def cross(d1: Pos, d2: Pos, d3: Pos) -> int or float:
    return (d2 - d1) // (d3 - d2)


def parallel(d1: Pos, d2: Pos, d3: Pos, d4: Pos) -> bool:
    return (d2 - d1) // (d4 - d3) == 0


def dot(d1: Pos, d2: Pos, d3: Pos) -> int or float:
    return (d2 - d1) ** (d3 - d2)


def on_seg(d1: Pos, d2: Pos, d3: Pos) -> bool:
    return cross(d1, d2, d3) == 0 and dot(d1, d3, d2) >= 0


def cross_check(d1: Pos, d2: Pos, d3: Pos, d4: Pos) -> bool:
    if parallel(d1, d2, d3, d4) and cross(d1, d2, d3) == 0:
        if d3 < d1 < d4 and d3 < d2 < d4:
            return False
        if d1 < d3 < d2 and d1 < d4 < d2:
            return False
    f1 = cross(d1, d2, d3) * cross(d2, d1, d4) > 0
    f2 = cross(d3, d4, d1) * cross(d4, d3, d2) > 0
    f3 = on_seg(d1, d2, d3) or \
        on_seg(d1, d2, d4) or \
        on_seg(d3, d4, d1) or \
        on_seg(d3, d4, d2)
    return (f1 and f2) or f3


def find_pos(s: int, q: int) -> Pos:
    if s == 1:
        return Pos(q, 0)
    elif s == 2:
        return Pos(q, 50)
    elif s == 3:
        return Pos(0, q)
    elif s == 4:
        return Pos(50, q)


if __name__ == '__main__':
    N = int(sys.stdin.readline().strip())
    SEG = []
    for _ in range(N // 2):
        s1, q1, s2, q2 = map(int, sys.stdin.readline().strip().split())
        p1, p2 = find_pos(s1, q1), find_pos(s2, q2)
        if p2 < p1:
            p1, p2 = p2, p1
        SEG.append([p1, p2])
    total = 0
    many = 0
    for i in range(N // 2):
        p1, p2 = SEG[i][0], SEG[i][1]
        cnt = 0
        for j in range(N // 2):
            if i != j:
                p3, p4 = SEG[j][0], SEG[j][1]
                cnt += cross_check(p1, p2, p3, p4)
        many = max(many, cnt)
        total += cnt
    # print(f'{total // 2}\n{many}')
