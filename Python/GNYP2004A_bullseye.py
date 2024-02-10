import sys
from math import hypot
TOL = 1e-7


class Pos:
    def __init__(self, x_, y_):
        self.x = x_
        self.y = y_

    def mag(self) -> float:
        return hypot(self.x, self.y)


def score(c: list) -> int:
    s = 0
    for p in c:
        if p.mag() < 3 + TOL:
            s += 100
        elif p.mag() < 6 + TOL:
            s += 80
        elif p.mag() < 9 + TOL:
            s += 60
        elif p.mag() < 12 + TOL:
            s += 40
        elif p.mag() < 15 + TOL:
            s += 20
        elif p.mag() > 15:
            s += 0
    return s


if __name__ == '__main__':
    sys.stdin = open('GNYP2004A.in', 'r')
    sys.stdout = open('GNYP2004A.out', 'w')
    while 1:
        coord = list(map(float, sys.stdin.readline().strip().split()))
        if coord[0] < -99:
            exit(0)
        P1 = [Pos(0, 0) for _ in range(3)]
        P2 = [Pos(0, 0) for _ in range(3)]
        for i in range(3):
            P1[i].x = coord[i * 2]
            P1[i].y = coord[i * 2 + 1]
        for i in range(3):
            P2[i].x = coord[i * 2 + 6]
            P2[i].y = coord[i * 2 + 7]
        S1 = score(P1)
        S2 = score(P2)
        winner = 'TIE.'
        if S1 > S2:
            winner = 'PLAYER 1 WINS.'
        elif S1 < S2:
            winner = 'PLAYER 2 WINS.'
        print(f'SCORE: {S1} to {S2}, {winner}')
