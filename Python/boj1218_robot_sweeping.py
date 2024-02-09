import sys


class Pos:
    def __init__(self, x_, y_):
        self.x = x_
        self.y = y_


LEN = 20_000
INF = int(1e17)
R, L, U, D = 1, 2, 4, 8
direction = [0] * LEN
N = 0
pos = [Pos(0, 0)] * LEN


def check(p: Pos) -> bool:
    l, r, u, d = -1, -1, -1, -1
    for i in range(N):
        cur, nxt = pos[i], pos[(i + 1) % N]
        if direction[i] == L:
            if cur.y < p.y and cur.x > p.x:
                if nxt.x < p.x or (nxt.x == p.x and direction[(i + 1) % N]) == D:
                    return False
            if cur.y >= p.y and cur.x >= p.x > nxt.x:
                if not ~u or cur.y > pos[u].y:
                    u = i
        if direction[i] == R:
            if cur.y > p.y and cur.x < p.x:
                if nxt.x > p.x or (nxt.x == p.x and direction[(i + 1) % N]) == U:
                    return False
            if cur.y <= p.y and cur.x <= p.x < nxt.x:
                if not ~d or cur.y < pos[d].y:
                    d = i
        if direction[i] == U:
            if cur.x < p.x and cur.y < p.y:
                if nxt.y > p.y or (nxt.y == p.y and direction[(i + 1) % N]) == L:
                    return False
            if cur.x >= p.x and cur.y <= p.y < nxt.y:
                if not ~r or cur.x > pos[r].x:
                    r = i
        if direction[i] == D:
            if cur.x > p.x and cur.y > p.y:
                if nxt.y < p.y or (nxt.y == p.y and direction[(i + 1) % N]) == R:
                    return False
            if cur.x <= p.x and cur.y >= p.y > nxt.y:
                if not ~l or cur.x < pos[l].x:
                    l = i
    i = d
    bound = -1
    pivot_y = pos[i].y
    while i != r:
        if direction[i] == D:
            bound = pos[i].x
        if direction[i] == U:
            pivot_y = max(pivot_y, pos[(i + 1) % N].y)
        if direction[i] == R:
            if pos[i].y < pivot_y:
                bound = pos[(i + 1) % N].x
        if direction[i] == L:
            if (i + 1) % N == r:
                i = (i + 1) % N
                continue
            if pos[(i + 1) % N].x < bound:
                return False
        i = (i + 1) % N
        # print('D')
    i = r
    bound = -1
    pivot_x = pos[i].x
    while i != u:
        if direction[i] == R:
            bound = pos[i].y
        if direction[i] == L:
            pivot_x = min(pivot_x, pos[(i + 1) % N].x)
        if direction[i] == U:
            if pos[i].x > pivot_x:
                bound = pos[(i + 1) % N].y
        if direction[i] == D:
            if (i + 1) % N == u:
                i = (i + 1) % N
                continue
            if pos[(i + 1) % N].y < bound:
                return False
        i = (i + 1) % N
        # print('R')
    i = u
    bound = INF
    pivot_y = pos[i].y
    while i != l:
        if direction[i] == U:
            bound = pos[i].x
        if direction[i] == D:
            pivot_y = min(pivot_y, pos[(i + 1) % N].y)
        if direction[i] == L:
            if pos[i].y > pivot_y:
                bound = pos[(i + 1) % N].x
        if direction[i] == R:
            if (i + 1) % N == l:
                i = (i + 1) % N
                continue
            if pos[(i + 1) % N].x > bound:
                return False
        i = (i + 1) % N
        # print('U')
    i = l
    bound = INF
    pivot_x = pos[i].x
    while i != d:
        if direction[i] == L:
            bound = pos[i].y
        if direction[i] == R:
            pivot_x = max(pivot_x, pos[(i + 1) % N].x)
        if direction[i] == D:
            if pos[i].x < pivot_x:
                bound = pos[(i + 1) % N].y
        if direction[i] == U:
            if (i + 1) % N == d:
                i = (i + 1) % N
                continue
            if pos[(i + 1) % N].y > bound:
                return False
        i = (i + 1) % N
        # print('L')
    # print('F')
    return True


if __name__ == '__main__':  # boj1218 algorithm from Jay22
    N = int(sys.stdin.readline().strip())
    pos = [Pos(*map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
    for j in range(N):
        CUR, NXT = pos[j], pos[(j + 1) % N]
        if CUR.x > NXT.x:
            direction[j] = L
        if CUR.x < NXT.x:
            direction[j] = R
        if CUR.y > NXT.y:
            direction[j] = D
        if CUR.y < NXT.y:
            direction[j] = U
    for _ in range(5):
        x, y = map(int, sys.stdin.readline().strip().split())
        if check(Pos(x, y)):
            print("YES")
        else:
            print("NO")
