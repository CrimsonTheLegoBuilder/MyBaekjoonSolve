import sys
from math import atan2, sin, cos, acos
from decimal import Decimal, getcontext

INPUT = sys.stdin.readline
fuck = Decimal(1)
scale: int = 1000000000000
PI = acos(-1)
getcontext().prec = 50
va = [[] for _ in range(100)]
F = [0] * 105


def sq(a):
    return a * a


def sign(x: [float, Decimal]) -> int:
    if x < 0:
        return -1
    if x > 0:
        return 1
    return 0


def norm(x) -> float:
    while x < 0:
        x += 2 * PI
    while x >= 2 * PI:
        x -= 2 * PI
    return x


def dist2(a, b):
    return sq(a[0] - b[0]) + sq(a[1] - b[1])


def cross_2(p1: list, p2: list):
    return p1[0] * p2[1] - p1[1] * p2[0]


def cross_3(p1: list, p2: list, p3: list):
    # return (p2[0] - p1[0]) * (p3[1] - p2[1]) - (p2[1] - p1[1]) * (p3[0] - p2[0])
    return Decimal(p2[0] - p1[0]) * Decimal(p3[1] - p2[1]) - Decimal(p2[1] - p1[1]) * Decimal(p3[0] - p2[0])


def ccw(p1: list, p2: list, p3: list) -> int:
    return sign(cross_3(p1, p2, p3))


def circle_circle_pos(a: list, b: list) -> int:
    d1: int = sq(a[0] - b[0]) + sq(a[1] - b[1])
    d2: int = sq(a[2] - b[2])
    d3: int = sq(a[2] + b[2])
    if d1 > d3:
        return -2
    if d1 == d3:
        return -1
    if d1 == d2:
        return 1
    if d1 < d2:
        return 2
    return 0


def circle_in(a: list, b: list) -> int:
    if a[0] == b[0] and a[1] == b[1] and a[2] == b[2]:
        return 1
    return a[0] > b[0] and circle_circle_pos(a, b) > 0


def intersection(a, b) -> list:
    ff = circle_circle_pos(a, b)
    if ff < 0 or 0 < ff:
        return []
    ca = [a[0], a[1]]
    cb = [b[0], b[1]]
    cv = [cb[0] - ca[0], cb[1] - ca[1]]
    ra = a[2]
    rb = b[2]
    rd = norm(atan2(cv[1], cv[0]))
    dsq = dist2(ca, cb)
    d = Decimal(dsq).sqrt()
    x = Decimal(sq(ra) - sq(rb) + dsq) / (Decimal(2 * ra) * d)
    if x < -1:
        x = -1
    if x > 1:
        x = 1
    h = acos(x)
    return [norm(rd - h), norm(rd + h)]


def pos(a: list, t: float):
    x = cos(t)
    y = sin(t)
    x *= a[2]
    y *= a[2]
    x += a[0]
    y += a[1]
    return [x, y]


def mid(a, b):
    return [(a[0] + b[0]) * .5, (a[1] + b[1]) * .5]


def green(c: list, lo: float, hi: float):
    l_ = [cos(lo), sin(lo)]
    h_ = [cos(hi), sin(hi)]
    fan = (hi - lo) * c[2] * c[2] * .5
    m = mid(l_, h_)
    m[0] *= c[2]
    m[0] += c[0]
    m[1] *= c[2]
    m[1] += c[1]
    tz = (cos(lo) - cos(hi)) * m[1] * c[2]
    return fan + tz - (cross_2(l_, h_)) * c[2] * c[2] * .5


def arc_init(vc) -> None:
    global va
    va = [[] for _ in range(100)]
    n = len(vc)
    for i in range(n):
        for j in range(n):
            # print("inx: ")
            if i == j:
                continue
            ff = circle_circle_pos(vc[i], vc[j])
            # print("inx: ", ff)
            if ff < 0 or 0 < ff:
                continue
            aa = [vc[i][0] / scale, vc[i][1] / scale, vc[i][2] / scale]
            bb = [vc[j][0] / scale, vc[j][1] / scale, vc[j][2] / scale]
            inx = intersection(aa, bb)
            # print("inx: ", inx)
            lo = inx[0]
            hi = inx[1]
            if lo > hi:
                a1 = [lo, 2 * PI, j]
                a2 = [0, hi, j]
                va[i].append(a1)
                va[i].append(a2)
            else:
                a1 = [lo, hi, j]
                va[i].append(a1)
        va[i].sort(key=lambda o: (o[0], o[1]))
        va[i].append([2 * PI, 2 * PI, -1])
    return


def union_up_to_x(vc: list, x: int):
    global va, F
    for i in range(len(F)):
        F[i] = 0
    for i in range(x + 1):
        for j in range(i + 1, x + 1):
            if F[j]:
                continue
            if circle_in(vc[i], vc[j]):
                F[j] = 1
    a = 0
    for i in range(x + 1):
        # hi = Decimal(0)
        hi = 0
        # print("i:", i)
        c = [vc[i][0] / scale, vc[i][1] / scale, vc[i][2] / scale]
        if F[i]:
            continue
        for arc in va[i]:
            # print("x & arc.i: ", x, arc[2])
            if arc[2] > x or F[arc[2]]:
                continue
            if hi < arc[0]:
                a += green(c, hi, arc[0])
                # print("arc: ", arc)
                # print("green: ", green(c, hi, arc[0]))
                # print("theta: ", hi, arc[0])
                hi = arc[1]
            else:
                hi = max(hi, arc[1])
    return a


def circle_area(a):
    return sq(a[2]) * PI


def eq(a, b):
    return abs(a - b) < 1e-13


def query() -> bool:
    global va
    n: int = int(INPUT())
    if not n:
        return False
    vc = []
    for i in range(n):
        ls = list(map(str, INPUT().split()))
        assert len(ls) == 3
        x = int(Decimal(ls[0]) * scale)
        y = int(Decimal(ls[1]) * scale)
        r = int(Decimal(ls[2]) * scale)
        vc.append([x, y, r])
        # print(ls[0], ls[1], ls[2])
        # print(vc[-1])
    vc.reverse()
    arc_init(vc)
    # for i in range(n):
        # vc[i][0] /= scale
        # vc[i][1] /= scale
        # vc[i][2] /= scale
        # print("va[", i, "]")
        # for aa in va[i]:
        #     print(aa)
    cnt: int = 1
    a = circle_area(vc[0])

    print("a: ", a)

    for i in range(1, n):
        aa = union_up_to_x(vc, i)

        print("a, aa: ", a, aa)

        if not eq(a, aa):
            cnt += 1
        a = aa
    print(cnt)
    return True


if __name__ == "__main__":
    while 1:
        f = query()
        if not f:
            break

'''

20
6.680085599158 7.011510259986 2.697815879981
3.376369230373 4.569784318725 2.524396111176
-3.476172463314 5.175644027295 3.301830037151
0.002861688970 4.782922948283 2.025085810404
-2.742889191745 6.261428822928 3.448985657967
6.124931814614 2.071116619077 1.770612229037
2.257101835919 -1.171517209960 3.461365852625
5.095221191808 -4.988154225517 3.695655232131
-1.012719642802 -4.147837851795 3.683656931769
1.264623529373 3.051424332588 0.443737043439
3.725234720086 4.467703489904 3.897606432039
5.855549255934 3.911720638193 0.446141148856
2.924066257977 -0.279239820400 1.011991677204
6.515406506526 -2.217101722088 3.298401060117
0.568429105390 -0.911076923149 1.700190683754
-2.987128928806 6.081423856507 1.044905179973
-3.895092944064 -2.568316983654 2.905045581851
-2.677962106405 4.713194740695 3.537712487130
-2.460502482733 -2.689276360225 3.461555430577
7.780342009959 0.366565980979 1.638131246998
0

'''