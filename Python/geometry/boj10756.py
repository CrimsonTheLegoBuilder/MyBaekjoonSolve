import sys  # boj 10756
MAX = int(1e17)


class Pos:
    def __init__(self, x_, y_):
        self.x = x_
        self.y = y_


def cross(d0: Pos, d1: Pos, d2: Pos) -> int:
    ret = (d1.x - d0.x) * (d2.y - d1.y) - (d1.y - d0.y) * (d2.x - d1.x)
    return ret


def monotone_chain() -> (list, list):  # return LOWER_HULL, UPPER_HULL
    C.sort(key=lambda pos: (pos.x, pos.y))
    if len(C) <= 2:
        return C, C[::-1]
    uh, dh = [], []
    for d in C:
        while len(dh) > 1 and cross(dh[-2], dh[-1], d) <= 0:
            dh.pop()
        dh.append(d)
    for d in reversed(C):
        while len(uh) > 1 and cross(uh[-2], uh[-1], d) <= 0:
            uh.pop()
        uh.append(d)
    return dh, uh


def ternary_search(hull: list, p1: Pos, p2: Pos) -> Pos:
    s, e = 0, len(hull) - 1
    dist = MAX
    ex = hull[0]
    while e - s > 2:
        ll = (s + s + e) // 3
        rr = (s + e + e) // 3
        cl = cross(p1, p2, hull[ll])
        cr = cross(p1, p2, hull[rr])
        if cl > cr:
            s = ll
        else:
            e = rr
    for i_ in range(s, e + 1):
        if (ccw := cross(p1, p2, hull[i_])) < dist:
            dist = ccw
            ex = hull[i_]
    return ex


def intersect(d1: Pos, d2: Pos, d3: Pos, d4: Pos) -> bool:
    return cross(d1, d2, d3) * cross(d1, d2, d4) > 0


def cross_check(cur: Pos, nxt: Pos) -> bool:
    p1, p2 = cur, nxt
    if p2.x < p1.x or (p1.x == p2.x and p2.y < p1.y):
        p1, p2 = p2, p1
    pl = ternary_search(LH, p1, p2)
    pu = ternary_search(UH, p2, p1)
    return intersect(cur, nxt, pl, pu)


if __name__ == "__main__":
    N = int(sys.stdin.readline().strip())
    pizza = [Pos(*map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
    M = int(sys.stdin.readline().strip())
    C = [Pos(*map(int, sys.stdin.readline().strip().split())) for _ in range(M)]
    LH, UH = monotone_chain()
    area, cur_area = 0, 0
    j = 1
    for i in range(N):  # two pointer
        while i != j and cross(pizza[i], pizza[j], LH[0]) > 0 and cross_check(pizza[i], pizza[j]):
            area = max(area, cur_area)
            j = (j + 1) % N
            a, b, c = pizza[i], pizza[j], pizza[j - 1]
            cur_area += abs(cross(a, b, c))
        a, b, c = pizza[i], pizza[(i + 1) % N], pizza[j]
        cur_area -= abs(cross(a, b, c))
    print(area)

# def dist(self, d1) -> int:
#     return (self.x - d1.x)**2 + (self.y - d1.y)**2

# def cross(self, d1) -> int:
#     ret = (self.x * d1.y) - (self.y * d1.x)
#     return ret
