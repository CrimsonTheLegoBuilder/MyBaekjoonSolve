import sys
import math
import random
from functools import cmp_to_key
INPUT = sys.stdin.readline

INF = int(1e17)
TOL = 1e-7


class Pos3D:
    def __init__(self, x: int = 0, y: int = 0, z: int = 0):
        self.x = x
        self.y = y
        self.z = z

    def __eq__(self, other) -> bool:
        return self.x == other.x and self.y == other.y and self.z == other.z

    def __ne__(self, other) -> bool:
        return not self.__eq__(other)

    def __lt__(self, other) -> bool:
        return (self.x, self.y, self.z) < (other.x, other.y, other.z)

    def __add__(self, other) -> 'Pos3D':
        return Pos3D(self.x + other.x, self.y + other.y, self.z + other.z)

    def __sub__(self, other) -> 'Pos3D':
        return Pos3D(self.x - other.x, self.y - other.y, self.z - other.z)

    def __mul__(self, scalar) -> 'Pos3D':
        return Pos3D(self.x * scalar, self.y * scalar, self.z * scalar)

    def __truediv__(self, scalar) -> 'Pos3D':
        return Pos3D(self.x / scalar, self.y / scalar, self.z / scalar)

    def __pow__(self, other) -> int:
        return self.x * other.x + self.y * other.y + self.z * other.z

    def __floordiv__(self, other) -> 'Pos3D':
        return Pos3D(self.y * other.z - self.z * other.y,
                     self.z * other.x - self.x * other.z,
                     self.x * other.y - self.y * other.x)

    def euc(self) -> int:
        return self.x * self.x + self.y * self.y + self.z * self.z

    def mag(self) -> float:
        return math.sqrt(self.euc())

    def __repr__(self) -> str:
        return f"{self.x} {self.y} {self.z}"


O3D = Pos3D(0, 0, 0)


def cross(d1: Pos3D, d2: Pos3D, d3: Pos3D) -> Pos3D:
    return (d2 - d1) // (d3 - d2)


def ccw(d1: Pos3D, d2: Pos3D, d3: Pos3D, nrm: Pos3D):
    torque = cross(d1, d2, d3)
    ret = torque ** nrm
    if not ret:
        return 0
    return 1 if ret > 0 else -1


norm = Pos3D(0, 0, 0)
pivot = Pos3D(0, 0, 0)


# def comp3d(d1: Pos3D, d2: Pos3D) -> bool:
#     f_ = ccw(pivot, d1, d2, norm)
#     if f_ == 0:
#         return (d1 - pivot).euc() < (d2 - pivot).euc()
#     else:
#         return f_ > 0


def comp3d(d1: Pos3D, d2: Pos3D) -> int:
    f_ = ccw(pivot, d1, d2, norm)
    # print("d1", d1.__repr__())
    # print("d2", d2.__repr__())
    # print("pivot", pivot.__repr__())
    # print("norm", norm.__repr__())
    # print(f_)
    if f_ == 0:
        a = (d1 - pivot).euc()
        b = (d2 - pivot).euc()
        if a == b:
            return 0
        elif a < b:
            return -1
        elif a > b:
            return 1
    else:
        return -1 if f_ > 0 else 1


def dot(d1: Pos3D, d2: Pos3D, d3: Pos3D) -> int:
    return (d2 - d1) ** (d3 - d2)


def on_seg_strong(d1: Pos3D, d2: Pos3D, d3: Pos3D) -> bool:
    return cross(d1, d2, d3) == 0 and dot(d1, d3, d2) >= 0


def inner_check(hull: list[Pos3D], nrm: Pos3D, p: Pos3D) -> int:
    sz = len(hull)
    col_ = 0
    for i in range(sz):
        cur = hull[i]
        nxt = hull[(i + 1) % sz]
        if ccw(cur, nxt, p, nrm) < 0:
            return -1
        if not ccw(cur, nxt, p, nrm):
            col_ = 1
    if col_:
        return 0
    return 1


def graham_scan(c: list[Pos3D], nrm: Pos3D, target: Pos3D) -> bool:
    global pivot, norm
    if len(c) < 3:
        c.sort()
        return on_seg_strong(c[0], c[-1], target)

    h = []
    i_ = c.index(min(c))
    c[0], c[i_] = c[i_], c[0]
    pivot, norm = c[0], nrm
    # for p in c:
    #     print(p.x, p.y, p.z, 1)
    c[1:] = sorted(c[1:], key=cmp_to_key(comp3d))
    # c[1:].sort(key=cmp_to_key(comp3d))
    # print(c[0].x, c[0].y, c[0].z)
    # for p in c:
    #     print(p.x, p.y, p.z, 0)

    # sz = len(c)
    # for i in range(sz):
    #     while len(h) >= 2 and ccw(h[-2], h[-1], c[i], nrm) <= 0:
    #         h.pop()
    #     h.append(c[i])
    for p in c:
        while len(h) >= 2 and ccw(h[-2], h[-1], p, nrm) <= 0:
            h.pop()
        h.append(p)
    # for p in h:
    #     print(p.x, p.y, p.z)

    # sz = len(h)
    # for i in range(sz):
    #     cur = h[i]
    #     nxt = h[(i + 1) % sz]
    #     if on_seg_strong(cur, nxt, p):
    #         return True
    # print(norm ** (target - h[0]))
    # print("vec", (target - h[0]).__repr__())
    # print("nrm", norm.__repr__())
    return (norm ** (target - h[0]) == 0) and inner_check(h, nrm, target) > -1


class Face:
    def __init__(self, a, b, c):
        self.v = [a, b, c]

    def norm(self, pos3d: list[Pos3D]) -> Pos3D:
        return cross(pos3d[self.v[0]], pos3d[self.v[1]], pos3d[self.v[2]])

    def visible(self, pos3d: list[Pos3D], i) -> bool:
        return (pos3d[i] - pos3d[self.v[0]]) ** self.norm(pos3d) > 0


# def collinear(a, b, c):
#     return ((b - a) // (c - b)).euc() == 0
#
#
# def coplanar(a, b, c, p):
#     return cross(a, b, c) ** (p - a) == 0
#
#
# def above(a, b, c, p):
#     return cross(a, b, c) ** (p - a) > 0
#
#
# def prep(p):
#     random.shuffle(p)
#     dim = 1
#     assert p[0] != O3D
#     for i in range(1, len(p)):
#         assert p[i] != O3D
#         if dim == 1:
#             if p[0] != p[i]:
#                 p[1], p[i] = p[i], p[1]
#                 dim += 1
#         elif dim == 2:
#             if not collinear(p[0], p[1], p[i]):
#                 p[2], p[i] = p[i], p[2]
#                 dim += 1
#         elif dim == 3:
#             if not coplanar(p[0], p[1], p[2], p[i]):
#                 p[3], p[i] = p[i], p[3]
#                 dim += 1
#     return dim
#
#
# col = True
# cop = True


# def convex_hull_3d(pos3d: list[Pos3D]) -> list[Face]:
#     global col, cop
#     col = False
#     cop = False
#     suf = prep(pos3d)
#     if suf == 2:
#         col = True
#         return []
#     if suf == 3:
#         cop = True
#         return []
#     sz = len(pos3d)
#     vis = [[0] * sz for _ in range(sz)]
#     cur = [Face(0, 1, 2), Face(2, 1, 0)]
#
#     for i in range(3, sz):
#         next_faces = []
#         for f in cur:
#             ret = f.visible(pos3d, i)
#             if not ret:
#                 next_faces.append(f)
#             for k in range(3):
#                 vis[f.v[k]][f.v[(k + 1) % 3]] = ret
#
#         for f in cur:
#             for k in range(3):
#                 a, b = f.v[k], f.v[(k + 1) % 3]
#                 if vis[a][b] != vis[b][a] and vis[a][b]:
#                     next_faces.append(Face(a, b, i))
#
#         cur = next_faces
#
#     return cur
#
#
# def cmp(c: list[Pos3D], face: list[int], p: Pos3D):
#     nrm = cross(c[face[0]], c[face[1]], c[face[2]])
#     ret = nrm ** (p - c[face[0]])
#     return 0 if not ret else (1 if ret > 0 else -1)
#
#
# def inner_check_3d(c: list[Pos3D], faces: list[Face], p: Pos3D):
#     cp = False
#     for face in faces:
#         ab = cmp(c, face.v, p)
#         if ab == 0:
#             cp = True
#         if ab > 0:
#             return -1
#     if cp:
#         return 0
#     return 1


def collinear(a, b, c):
    return ((b - a) // (c - b)).euc() == 0


def coplanar(a, b, c, p):
    return cross(a, b, c) ** (p - a) == 0


def above(a, b, c, p):
    return cross(a, b, c) ** (p - a) > 0


def prep(p):
    random.shuffle(p)
    dim = 1
    assert p[0] != O3D
    for i in range(1, len(p)):
        assert p[i] != O3D
        if dim == 1:
            if p[0] != p[i]:
                p[1], p[i] = p[i], p[1]
                dim += 1
        elif dim == 2:
            if not collinear(p[0], p[1], p[i]):
                p[2], p[i] = p[i], p[2]
                dim += 1
        elif dim == 3:
            if not coplanar(p[0], p[1], p[2], p[i]):
                p[3], p[i] = p[i], p[3]
                dim += 1
    return dim


col = True
cop = True


def convex_hull_3d(candi: list[Pos3D]) -> list[list[int]]:
    global col, cop
    col = False
    cop = False
    suf = prep(candi)
    if suf == 2:
        col = True
        return []
    if suf == 3:
        cop = True
        return []
    sz = len(candi)
    faces = []
    active = []
    vis = [[] for _ in range(sz)]
    rvis = []
    other = []

    def ad(a: int, b: int, c: int) -> None:
        faces.append([a, b, c])
        active.append(True)
        rvis.append([])
        other.append([0, 0, 0])

    def visible(a: int, b: int) -> None:
        vis[b].append(a)
        rvis[a].append(b)

    def abv(a: int, b: int) -> bool:
        tri = faces[a]
        return above(candi[tri[0]], candi[tri[1]], candi[tri[2]], candi[b])

    def edge(e: tuple) -> tuple:
        return faces[e[0]][e[1]], faces[e[0]][(e[1] + 1) % 3]

    def glue(a: tuple, b: tuple) -> None:
        ed = edge(a)
        assert edge(b) == (ed[1], ed[0])
        other[a[0]][a[1]] = b
        other[b[0]][b[1]] = a

    ad(0, 1, 2)
    ad(0, 2, 1)
    if abv(1, 3):
        candi[1], candi[2] = candi[2], candi[1]
    for i in range(3):
        glue((0, i), (1, 2 - i))
    for i in range(3, N):
        visible(abv(1, i), i)
    label = [-1] * sz
    for i in range(3, sz):
        rem = []
        for v in vis[i]:
            if active[v]:
                active[v] = False
                rem.append(v)
        if not rem:
            continue
        st = -1
        for v in rem:
            for j in range(3):
                o = other[v][j][0]
                if active[o]:
                    idx1, idx2 = edge((v, j))
                    ad(idx1, idx2, i)
                    st = idx1
                    cur = len(rvis) - 1
                    label[idx1] = cur
                    tmp = list(set(rvis[v]) | set(rvis[o]))
                    for x in tmp:
                        # print(cur)
                        # print(len(faces))
                        if abv(cur, x):
                            visible(cur, x)
                    glue((cur, 0), other[v][j])
        x = st
        while True:
            idx = label[x]
            y = faces[idx][1]
            glue((idx, 1), (label[y], 2))
            y = faces[idx][1]
            if y == st:
                break
            x = y
    hull3d = [faces[i] for i in range(len(faces)) if active[i]]
    return hull3d


def cmp(c: list[Pos3D], face: list[int], p: Pos3D):
    nrm = cross(c[face[0]], c[face[1]], c[face[2]])
    ret = nrm ** (p - c[face[0]])
    return 0 if not ret else (1 if ret > 0 else -1)


def inner_check_3d(c: list[Pos3D], faces: list[list[int]], p: Pos3D):
    cp = False
    for face in faces:
        ab = cmp(c, face, p)
        if ab == 0:
            cp = True
        if ab > 0:
            return -1
    if cp:
        return 0
    return 1


if __name__ == '__main__':
    # sys.stdin = open('ts2_input.txt', 'r')
    # sys.stdout = open('ts2_output.txt', 'w')
    T = int(INPUT())
    for tc in range(1, T + 1):
        N = int(INPUT())
        C = [Pos3D(*map(int, INPUT().strip().split())) for _ in range(N)]
        Hull3D = convex_hull_3d(C)
        if col:
            C.sort(key=lambda c: (c.x, c.y, c.z))
            f = on_seg_strong(C[0], C[-1], O3D)
            ans = "YES" if f else "NO"
            print(f"Case #{tc}:", ans)
        elif cop:
            norm = cross(C[0], C[1], C[2])
            f = graham_scan(C, norm, O3D)
            ans = "YES" if f else "NO"
            print(f"Case #{tc}:", ans)
        else:
            f = inner_check_3d(C, Hull3D, O3D)
            ans = "YES" if f > -1 else "NO"
            print(f"Case #{tc}:", ans)

