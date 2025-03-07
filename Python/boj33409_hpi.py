import sys
from fractions import Fraction
INPUT = sys.stdin.readline


def read_fraction() -> tuple:
    p_x, q_x, p_y, q_y = map(int, INPUT().split())
    return Fraction(p_x, q_x), Fraction(p_y, q_y)


def cross(d1, d2, d3) -> Fraction:
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def intersection_(p1_, p2_, q1_, q2_) -> tuple:
    a1 = cross(q1_, q2_, p1_)
    a2 = -cross(q1_, q2_, p2_)
    x: Fraction = p1_[0] * a2 + p2_[0] * a1
    y: Fraction = p1_[1] * a2 + p2_[1] * a1
    den: Fraction = a1 + a2
    return Fraction(x, den), Fraction(y, den)


N: int

if __name__ == "__main__":
    N = int(INPUT())
    K: int = int((N * 2)**.5 + 1)
    # print(K)

    H: list = []
    for i in range(N):
        p: tuple = read_fraction()
        H.append(p)

    L: list = []
    V: set = set()
    for i in range(N):
        for j in range(N):
            if j == i:
                continue
            I: tuple = H[i]
            J: tuple = H[j]
            r: int = 0
            z: int = 0
            l: int = 0
            for k in range(N):
                if k == i or k == j:
                    continue
                tq: Fraction = cross(I, J, H[k])
                if tq == 0:
                    z += 1
                if tq > 0:
                    l += 1
                if tq < 0:
                    r += 1
            if z == K - 1:
                if l < r:
                    I, J = J, I
