import sys


class Pos:
    def __init__(self, x_, y_, ccw_, cw_):
        self.x = x_
        self.y = y_
        self.ccw = ccw_
        self.cw = cw_

    def cross(self, pos1, pos2):
        ret = (pos1.x - self.x) * (pos2.y - pos1.y) - (pos1.y - self.y) * (pos2.x - pos1.x)
        return ret


class Order:
    def __init__(self, i_, coord_):
        self.i = i_
        self.coord = coord_


N = int(sys.stdin.readline().strip())

ans = [0] * N
V = [0] * N
order = [0] * N

pos = [Pos(0, 0, 0, 0)] * N
LR = [Order(0, 0)] * N
DU = [Order(0, 0)] * N

for i in range(N):
    x, y = map(int, sys.stdin.readline().strip().split())
    pos[i] = Pos(x, y, 0, 0)
    LR[i] = Order(i, x)
    DU[i] = Order(i, y)

LR.sort(key=lambda o: o.coord)
DU.sort(key=lambda o: o.coord)

S = sys.stdin.readline().strip()

L, R, D, U = 0, N - 1, 0, N - 1
for i in range(len(S)):
    if S[i] == 'L':
        while L <= R and V[LR[L].i]:
            L += 1
        order[i] = LR[L].i

    if S[i] == 'R':
        while L <= R and V[LR[R].i]:
            R -= 1
        order[i] = LR[R].i

    if S[i] == 'D':
        while D <= U and V[DU[D].i]:
            D += 1
        order[i] = DU[D].i

    if S[i] == 'U':
        while D <= U and V[DU[U].i]:
            U -= 1
        order[i] = DU[U].i

    V[order[i]] = 1

R = LR[0].i
L = LR[N - 1].i
U = DU[0].i
D = DU[N - 1].i

for i in range(N):
    if not V[i]:
        if pos[i].x < pos[L].x:
            L = i
        if pos[i].x > pos[R].x:
            R = i
        if pos[i].y < pos[D].y:
            D = i
        if pos[i].y > pos[U].y:
            U = i

pos[R].ccw = pos[R].cw = L
pos[L].ccw = pos[L].cw = R

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

    k = order[i]
    pos[k].ccw = pos[k].cw = j

    while (area := pos[k].cross(pos[pos[k].ccw], pos[pos[pos[k].ccw].ccw])) > 0:
        # print(area)
        ans[i] += area
        pos[k].ccw = pos[pos[k].ccw].ccw
    while (area := pos[k].cross(pos[pos[k].cw], pos[pos[pos[k].cw].cw])) < 0:
        # print(area)
        ans[i] -= area
        pos[k].cw = pos[pos[k].cw].cw

    pos[pos[k].ccw].cw = k
    pos[pos[k].cw].ccw = k

    if pos[k].x < pos[L].x:
        L = k
    if pos[k].x > pos[R].x:
        R = k
    if pos[k].y < pos[D].y:
        D = k
    if pos[k].y > pos[U].y:
        U = k

    ans[i] += ans[i + 1]

for i in range(N - 2):
    print(ans[i] / 2)






