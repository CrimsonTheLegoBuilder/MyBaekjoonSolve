import sys
from fractions import Fraction
from functools import cmp_to_key
INPUT = sys.stdin.readline


def read_fraction() -> tuple:
    p_x, q_x, p_y, q_y = map(int, INPUT().split())
    return Fraction(p_x, q_x), Fraction(p_y, q_y)


def cross(d1, d2, d3) -> Fraction:
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def intersection(p1_, p2_, q1_, q2_) -> tuple:
    a1 = cross(q1_, q2_, p1_)
    a2 = -cross(q1_, q2_, p2_)
    x_: Fraction = p1_[0] * a2 + p2_[0] * a1
    y_: Fraction = p1_[1] * a2 + p2_[1] * a1
    den: Fraction = a1 + a2
    return Fraction(x_, den), Fraction(y_, den)


def cmp_(t1_, t2_) -> int:
    p1_ = (t1_[0][0] - t1_[1][0], t1_[0][1] - t1_[1][1])
    p2_ = (t2_[0][0] - t2_[1][0], t2_[0][1] - t2_[1][1])
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
    N = int(INPUT())
    K: int = int((N * 2)**.5 + 1)
    # print(K)

    H: list = []
    for i in range(N):
        p: tuple = read_fraction()
        H.append(p)

    # print(len(H))

    L: list = []
    V: list = []
    for i in range(N):
        for j in range(i + 1, N):
            if j == i:
                continue
            I: tuple = H[i]
            J: tuple = H[j]
            x: Fraction = J[0] - I[0]
            y: Fraction = J[1] - I[1]
            t: tuple = x, y
            f: bool = False
            for v in V:
                det: Fraction = t[0] * v[1] - t[1] * v[0]
                if det == 0:
                    f = True
                    break
            if f:
                continue
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
            # print("z", z, "l", l, "r", r)
            if z == K - 3:
                if l < r:
                    I, J = J, I
                x: Fraction = J[0] - I[0]
                y: Fraction = J[1] - I[1]
                L.append((I, J))
                V.append((x, y))

    # print(len(L))
    # print(L)
    R: list = []
    L.sort(key=cmp_to_key(cmp_))
    for i in range(len(L)):
        I: tuple = L[i - 1]
        J: tuple = L[i]
        R.append(intersection(I[0], I[1], J[0], J[1]))

    # print(R)

    print(len(R))
    for ret in R:
        print(ret[0].numerator, ret[0].denominator, ret[1].numerator, ret[1].denominator)
