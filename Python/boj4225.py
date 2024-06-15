import sys
from math import hypot, ceil
INPUT = sys.stdin.readline
TOL: float = 1e-7
INF: float = 1e17


class Pos:
    def __init__(self, x_: int, y_: int):
        self.x = x_
        self.y = y_

    def __sub__(self, p: 'Pos') -> 'Pos':
        return Pos(self.x - p.x, self.y - p.y)

    def __floordiv__(self, p: 'Pos') -> int or float:
        return self.x * p.y - self.y * p.x

    def __pow__(self, p: 'Pos') -> int or float:
        return self.x * p.x + self.y * p.y

    def mag(self) -> float:
        return hypot(self.x, self.y)

    def euc(self) -> int or float:
        return self.x * self.x + self.y * self.y


def cross(d1: Pos, d2: Pos, d3: Pos) -> int:
    return (d2 - d1) // (d3 - d2)


def monotone_chain(candi: list[Pos]) -> list[Pos]:
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


def ccw(hull: list[Pos], i_: int, f: int) -> int or float:
    sz_ = len(hull)
    i1, i2 = hull[i_], hull[(i_ + 1) % sz_]
    f1, f2 = hull[f % sz_], hull[(f + 1) % sz_]
    return (i2 - i1) // (f2 - f1)


def dist(p1: Pos, p2: Pos, q: Pos) -> float:
    return cross(p1, p2, q) / (p1 - p2).mag()


if __name__ == '__main__':
    tc: int = 0
    while 1:
        tc += 1
        N: int = int(INPUT().strip())
        if not N:
            break
        C: list[Pos] = [Pos(*map(int, INPUT().strip().split())) for _ in range(N)]
        H: list[Pos] = monotone_chain(C)
        ans: float = INF
        sz: int = len(H)
        j: int = 0
        for i in range(sz):
            while ccw(H, i, j) >= 0:
                j = (j + 1) % sz
            ans = min(ans, dist(H[i], H[(i + 1) % sz], H[j]))
        print(f'Case {tc}: {ceil(ans * 100) / 100:.2f}')
