import sys


class Pos:
    def __init__(self, x_, y_):
        self.x = x_
        self.y = y_

    def cross(self, d1, d2):
        ret = (d1.x - self.x) * (d2.y - d1.y) - (d1.y - self.y) * (d2.x - d1.x)
        return ret

    def dist(self, d1):
        return ((self.x - d1.x)**2 + (self.y - d1.y)**2)**.5


def monotone_chain():
    D.sort(key=lambda pos: (pos.x, pos.y))
    uh, dh = [], []
    for d in D:
        while len(dh) > 1 and Pos.cross(dh[-2], dh[-1], d) <= 0:
            dh.pop()
        dh.append(d)
    for d in reversed(D):
        while len(uh) > 1 and Pos.cross(uh[-2], uh[-1], d) <= 0:
            uh.pop()
        uh.append(d)
    return dh[:-1] + uh[:-1]


N = int(sys.stdin.readline().strip())
D = [Pos(*map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
hull = monotone_chain()

circ = 0
for i in range(len(hull)):
    cur, nxt = hull[i - 1], hull[i]
    circ += Pos.dist(cur, nxt)

print(f'{circ:.2f}')
