import sys
from fractions import Fraction
from functools import cmp_to_key
INPUT = sys.stdin.readline


def read_fraction() -> tuple:
    p_x, q_x, p_y, q_y = map(int, INPUT().split())
    return Fraction(p_x, q_x), Fraction(p_y, q_y)


def cross(d1: tuple, d2: tuple, d3: tuple) -> Fraction:
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def intersection(p1_: tuple, p2_: tuple, q1_: tuple, q2_: tuple) -> tuple:
    a1: Fraction = cross(q1_, q2_, p1_)
    a2: Fraction = cross(q1_, q2_, p2_) * Fraction(-1, 1)
    x_: Fraction = p1_[0] * a2 + p2_[0] * a1
    y_: Fraction = p1_[1] * a2 + p2_[1] * a1
    den: Fraction = a1 + a2
    return Fraction(x_, den), Fraction(y_, den)


def collinear(l1_: tuple, l2_: tuple) -> bool:
    tq1: Fraction = cross(l1_[0], l1_[1], l2_[0])
    tq2: Fraction = cross(l1_[0], l1_[1], l2_[1])
    return tq1 == 0 and tq2 == 0


def cmp_(t1_: tuple, t2_: tuple) -> int:
    p1_: tuple = (t1_[0][0] - t1_[1][0], t1_[0][1] - t1_[1][1])
    p2_: tuple = (t2_[0][0] - t2_[1][0], t2_[0][1] - t2_[1][1])
    f1: bool = (0, 0) < p1_
    f2: bool = (0, 0) < p2_
    if f1 != f2:
        return -1 if f1 > f2 else 1
    det_: Fraction = p1_[0] * p2_[1] - p1_[1] * p2_[0]
    if det_ == 0:
        d: Fraction = cross(t1_[0], t1_[1], t2_[0])
        if d == 0:
            return 0
        return -1 if d > 0 else 1
    return -1 if det_ > 0 else 1


if __name__ == "__main__":
    N: int = int(INPUT())
    K: int = int((N * 2)**.5 + 1)

    H: list = []
    for i in range(N):
        p: tuple = read_fraction()
        H.append(p)

    L: list = []
    for i in range(N):
        for j in range(i + 1, N):
            p0: tuple = H[i]
            p1: tuple = H[j]
            if p1 < p0:
                p0, p1 = p1, p0
            L.append((p0, p1))

    L.sort(key=cmp_to_key(cmp_))

    sz: int = len(L)
    cnt: int = 1
    M: int = (K - 1) * (K - 2) // 2
    V: list = []
    for i in range(sz):
        l0: tuple = L[i]
        l1: tuple = L[(i + 1) % sz]
        if collinear(l0, l1):
            cnt += 1
        else:
            if cnt == M:
                r: int = 0
                l: int = 0
                p0: tuple = l0[0]
                p1: tuple = l0[1]
                for j in range(N):
                    tq: Fraction = cross(p0, p1, H[j])
                    if tq == 0:
                        continue
                    elif tq > 0:
                        l += 1
                    else:
                        r += 1
                if l < r:
                    p0, p1 = p1, p0
                V.append((p0, p1))
                cnt = 1
            else:
                cnt = 1
                continue

    V.sort(key=cmp_to_key(cmp_))

    R: list = []
    for i in range(len(V)):
        l0: tuple = V[i - 1]
        l1: tuple = V[i]
        R.append(intersection(l0[0], l0[1], l1[0], l1[1]))

    print(len(R))
    for ret in R:
        print(ret[0].numerator, ret[0].denominator, ret[1].numerator, ret[1].denominator)
