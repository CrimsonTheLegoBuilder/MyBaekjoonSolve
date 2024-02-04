import sys
input = sys.stdin.readline
class Pos:
    def __init__(self, x_, y_, c, w):
        self.x = x_
        self.y = y_
        self.c = c
        self.w = w
    def cr(self, d1, d2):
        r = (d1.x - self.x) * (d2.y - d1.y) - (d1.y - self.y) * (d2.x - d1.x)
        return r
class Order:
    def __init__(self, i, c):
        self.i = i
        self.c = c
N = int(input())
A = [0] * N
V = [0] * N
O = [0] * N
P = [Pos(0, 0, 0, 0)] * N
LR = [Order(0, 0)] * N
DU = [Order(0, 0)] * N
for i in range(N):
    x, y = map(int, input().split())
    P[i] = Pos(x, y, 0, 0)
    LR[i] = Order(i, x)
    DU[i] = Order(i, y)
S = input().strip()
LR.sort(key=lambda o: o.c)
DU.sort(key=lambda o: o.c)
L, R = 0, N - 1
D, U = 0, N - 1
for i in range(len(S)):
    if S[i] == 'L':
        while L <= R and V[LR[L].i]:
            L += 1
        O[i] = LR[L].i
    if S[i] == 'R':
        while L <= R and V[LR[R].i]:
            R -= 1
        O[i] = LR[R].i
    if S[i] == 'D':
        while D <= U and V[DU[D].i]:
            D += 1
        O[i] = DU[D].i
    if S[i] == 'U':
        while D <= U and V[DU[U].i]:
            U -= 1
        O[i] = DU[U].i
    V[O[i]] = 1
L = LR[N - 1].i
R = LR[0].i
D = DU[N - 1].i
U = DU[0].i
for i in range(N):
    if not V[i]:
        if P[i].x < P[L].x:
            L = i
        if P[i].x > P[R].x:
            R = i
        if P[i].y < P[D].y:
            D = i
        if P[i].y > P[U].y:
            U = i
P[R].c = P[R].w = L
P[L].c = P[L].w = R
for i in range(N - 3, -1, -1):
    j = -1
    if S[i] == 'L':
        j = L
    if S[i] == 'R':
        j = R
    if S[i] == 'D':
        j = D
    if S[i] == 'U':
        j = U
    k = O[i]
    P[k].c = P[k].w = j
    while (a := P[k].cr(P[P[k].c], P[P[P[k].c].c])) > 0:
        A[i] += a
        P[k].c = P[P[k].c].c
    while (a := P[k].cr(P[P[k].w], P[P[P[k].w].w])) < 0:
        A[i] -= a
        P[k].w = P[P[k].w].w
    P[P[k].c].w = k
    P[P[k].w].c = k
    if P[k].x < P[L].x:
        L = k
    if P[k].x > P[R].x:
        R = k
    if P[k].y < P[D].y:
        D = k
    if P[k].y > P[U].y:
        U = k
    A[i] += A[i + 1]
for i in range(N-2):  # boj2964
    print(f"{A[i]//2}.{(A[i]&1)*5}")
