import sys
from math import gcd
from functools import cmp_to_key
INPUT = sys.stdin.readline


def sign(x):
    if x < 0:
        return -1
    if x > 0:
        return 1
    return 0


def cross(p1, p2, p3):
    return (p2[0] - p1[0]) * (p3[1] - p2[1]) - (p2[1] - p1[1]) * (p3[0] - p2[0])


def ccw(p1, p2, p3):
    return sign(cross(p1, p2, p3))


def cross4(p1, p2,
           p3, p4):
    return (p2[0] - p1[0]) * (p4[1] - p3[1]) - (p2[1] - p1[1]) * (p4[0] - p3[0])


def ccw4(p1, p2,
         p3, p4):
    return sign(cross4(p1, p2, p3, p4))


def intersection(p1, p2,
                 q1, q2
                 ):
    c1: int = cross(q1, q2, p1)
    c2: int = -cross(q1, q2, p2)
    x: int = p1[0] * c2 + p2[0] * c1
    det: int = c1 + c2
    d = gcd(x, det)
    if d == 0:
        return [0, 1, 0]
    x //= d
    det //= d
    if not x:
        det = abs(det)
    elif x * det < 0 < x:
        x *= -1
        det *= -1
    elif x * det > 0:
        x = abs(x)
        det = abs(det)
    return [x, det, 0]


def area(h):
    a: int = 0
    n: int = len(h)
    for i_ in range(n):
        a += cross([0, 0], h[i_ - 1], h[i_])
    return a


def cmp_list(p1, p2):
    s1: int = sign(p1[0] * p1[1])
    s2: int = sign(p2[0] * p2[1])
    if s1 != s2:
        return -1 if s1 < s2 else 1
    j1: int = abs(p1[0] * p2[1])
    j2: int = abs(p2[0] * p1[1])
    if len(p1) == 3 and j1 == j2:
        return -1 if p1[2] < p2[2] else 1
    if not s1 and not s2:
        return 0
    if s1 > 0:
        if j1 < j2:
            return -1
        elif j1 == j2:
            return 0
        return 1
    if j1 < j2:
        return 1
    elif j1 == j2:
        return 0
    return -1


def cmp_tuple(p1, p2):
    f1: int = p2[0] - p1[0]
    f2: int = p2[1] - p1[1]
    if not f1:
        return sign(f2) * -1
    return -1 if f1 > 0 else 1


if __name__ == "__main__":
    N, vx, vy = map(int, INPUT().strip().split())
    vx *= -1
    vy *= -1
    P = []
    for i in range(N):
        tmp = list(map(int, INPUT().strip().split()))
        assert len(tmp) > 0
        M = tmp[0]
        assert M > 0
        H = []
        for j in range(1, M + 1):
            H.append([tmp[j * 2 - 1], tmp[j * 2]])
        P.append(H[:])
    if vx < 0:
        vx *= -1
        for i in range(len(P)):
            for j in range(len(P[i])):
                P[i][j][0] *= -1
    elif not vx:
        if vy < 0:
            vx, vy = -vy, vx
            for i in range(len(P)):
                for j in range(len(P[i])):
                    P[i][j][0], P[i][j][1] = -P[i][j][1], P[i][j][0]
        elif vy > 0:
            vx, vy = vy, -vx
            for i in range(len(P)):
                for j in range(len(P[i])):
                    P[i][j][0], P[i][j][1] = P[i][j][1], -P[i][j][0]

    for i in range(len(P)):
        A = area(P[i])
        if A < 0:
            P[i] = P[i][::-1][:]

    R = []
    S = set()
    D = {}
    s = [0, 0]
    e = [vx, vy]
    for H in P:
        V = []
        M = len(H)
        for j in range(M):
            d0 = H[j - 2]
            d1 = H[j - 1]
            d2 = H[j]
            # print(d0, d1, d2)
            det0 = ccw(s, e, d0)
            det1 = ccw(s, e, d1)
            det2 = ccw(s, e, d2)
            if det1 * det2 < 0:
                inx = intersection(s, e, d1, d2)
                if ccw4(s, e, d1, d2) < 0:
                    inx[2] = 0
                else:
                    inx[2] = 1
                V.append(inx[:])
            elif not det1:
                inx = [d1[0], 1, 0]
                if det0 * det2 > 0:
                    if ccw(d0, d1, d2) > 0:
                        inx[2] = 0
                        # print(inx)
                        V.append(inx[:])
                        inx[2] = 1
                        # print(inx)
                        V.append(inx[:])
                        # print("V:: ", V)
                    continue
                # print("fuck::")
                if det0 > 0 or det2 < 0:
                    inx[2] = 0
                elif det0 < 0 or det2 > 0:
                    inx[2] = 1
                if inx[2] == 0:
                    if not det0 and ccw(d0, d1, d2) < 0:
                        continue
                    if not det2 and ccw(d0, d1, d2) < 0:
                        continue
                if inx[2] == 1:
                    if not det0 and ccw(d0, d1, d2) < 0:
                        continue
                    if not det2 and ccw(d0, d1, d2) < 0:
                        continue
                # print("suck::")
                V.append(inx[:])
        V.sort(key=cmp_to_key(cmp_list))
        M = len(V)
        for j in range(0, M, 2):
            r1 = V[j]
            r2 = V[j + 1]
            # print(r1, r2)
            assert r1[2] == 0
            assert r2[2] == 1
            r1.pop()
            r2.pop()
            if r2[0] * r2[1] < 0:
                continue
            R.append((tuple(r1), tuple(r2)))
            S.add(tuple(r1))
            S.add(tuple(r2))

    # print(S)
    K = list(S)
    K.sort(key=cmp_to_key(cmp_list))
    # print(K)
    M = len(K)
    for i, t in enumerate(K):
        D[t] = i

    U = []
    for i in range(len(R)):
        r = R[i]
        d1 = r[0]
        d2 = r[1]
        if d1 not in D or d2 not in D:
            continue
        i1 = D[d1]
        i2 = D[d2]
        U.append((i1, i2))

    U.sort(key=cmp_to_key(cmp_tuple))
    # print(U)

    cnt = 0
    hi = -1
    for t in U:
        if hi < t[0]:
            cnt += 1
            hi = t[1]
        else:
            hi = max(hi, t[1])

    print(cnt)

'''

2 -1 0
3 1 0 2 0 1 2
3 1 0 2 0 1 2

'''