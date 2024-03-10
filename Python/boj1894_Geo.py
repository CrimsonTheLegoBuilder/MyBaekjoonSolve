import sys
from math import hypot
TOL = 1e-7
INPUT = sys.stdin.readline


def zero(x: float) -> bool:
    return abs(x) < TOL


class Pos:
    def __init__(self, x_: float, y_: float):
        self.x = x_
        self.y = y_

    def __eq__(self, p: 'Pos') -> bool:
        return zero(self.x - p.x) and zero(self.y - p.y)

    def __add__(self, p: 'Pos') -> 'Pos':
        return Pos(self.x + p.x, self.y + p.y)

    def __sub__(self, p: 'Pos') -> 'Pos':
        return Pos(self.x - p.x, self.y - p.y)

    def __floordiv__(self, p: 'Pos') -> float:
        return self.x * p.y - self.y * p.x

    def __pow__(self, p: 'Pos') -> float:
        return self.x * p.x + self.y * p.y

    def mag(self) -> float:
        return hypot(self.x, self.y)

    def euc(self) -> float:
        return self.x * self.x + self.y * self.y

    def __repr__(self):
        return f"{self.x:.3f} {self.y:.3f}"


def cross(d1: Pos, d2: Pos, d3: Pos) -> int or float:
    return (d2 - d1) // (d3 - d2)


if __name__ == '__main__':  # boj1894
    while 1:
        try:
            coord = list(map(float, INPUT().strip().split()))
            pos = [Pos(0, 0) for _ in range(4)]
            for i in range(4):
                pos[i] = Pos(coord[2 * i], coord[2 * i + 1])
            p1 = pos[0]
            p2 = pos[1]
            p3 = pos[2]
            p4 = pos[3]
            if not p1 == p3 and not p2 == p3:
                p3, p4 = p4, p3
            if not p2 == p3:
                p1, p2 = p2, p1
            p5 = p1 + (p4 - p3)
            # print(p5)
        except:
            break
