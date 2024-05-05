import sys
from math import hypot, pi
TOL = 1e-7


class Pos:
    def __init__(self, x_: float, y_: float):
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


def cross(d1: Pos, d2: Pos, d3: Pos) -> int or float:
    return (d2 - d1) // (d3 - d2)


# p1: Pos = Pos(17, 7)
# p2: Pos = Pos(19, 90)
# vec = p2 - p1
# h = Pos(-vec.y, vec.x)
# l = h.mag()
# h = Pos(h.x / l * 100, h.y / l * 100)
# print(cross(p1, p2, h) / 2)

print((100 + 25) * pi)