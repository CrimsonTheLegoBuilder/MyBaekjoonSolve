import sys
from collections import deque


def range_check(d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11):
    return 0 <= d1 < W and 0 <= d2 < V and 0 <= d3 < U and 0 <= d4 < T and 0 <= d5 < S and 0 <= d6 < R and 0 <= d7 < Q and \
            0 <= d8 < P and 0 <= d9 < O and 0 <= d10 < N and 0 <= d11 < M


def tomato_check(d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11):
    return box[d1][d2][d3][d4][d5][d6][d7][d8][d9][d10][d11] == 0


dw = [1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
dv = [0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
du = [0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
dt = [0, 0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
ds = [0, 0, 0, 0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
dr = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
dq = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0]
dp = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0]
do = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0]
dn = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1, 0, 0]
dm = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1]


def bfs():
    day = 0
    zero = 0
    while queue:
        a, b, c, d, e, f, g, h, i, j, k, day = queue.popleft()
        for x in range(22):
            nw, nv, nu, nt, ns, nr, nq, np, no, nn, nm = a + dw[x], b + dv[x], c + du[x], d + dt[x], e + ds[x], f + dr[x], g + dq[x], h + dp[x], i + do[x], j + dn[x], k + dm[x]
            if range_check(nw, nv, nu, nt, ns, nr, nq, np, no, nn, nm) and tomato_check(nw, nv, nu, nt, ns, nr, nq, np, no, nn, nm):
                queue.append((nw, nv, nu, nt, ns, nr, nq, np, no, nn, nm, day + 1))
                box[nw][nv][nu][nt][ns][nr][nq][np][no][nn][nm] = 1
                zero += 1
    # print(zero)
    return day if Z == zero else -1


M, N, O, P, Q, R, S, T, U, V, W = map(int, sys.stdin.readline().strip().split())
Z = 0
box = [
    [
        [
            [
                [
                    [
                        [
                            [
                                [
                                    [
                                        list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)
                                    ] for _ in range(O)
                                ] for _ in range(P)
                            ] for _ in range(Q)
                        ] for _ in range(R)
                    ] for _ in range(S)
                ] for _ in range(T)
            ] for _ in range(U)
        ] for _ in range(V)
    ] for _ in range(W)
]

queue = deque()
for w in range(W):
    for v in range(V):
        for u in range(U):
            for t in range(T):
                for s in range(S):
                    for r in range(R):
                        for q in range(Q):
                            for p in range(P):
                                for o in range(O):
                                    for n in range(N):
                                        for m in range(M):
                                            if box[w][v][u][t][s][r][q][p][o][n][m] == 1:
                                                queue.append((w, v, u, t, s, r, q, p, o, n, m, 0))
                                            if box[w][v][u][t][s][r][q][p][o][n][m] == 0:
                                                Z += 1

# print(Z)
print(bfs())

'''
2 2 2 1 1 1 1 1 1 1 1
-1 1
1 -1
-1 1
0 -1
1

3 2 1 1 1 1 1 1 1 1 1
-1 0 0
-1 1 0
2

2 2 1 1 1 1 1 1 1 1 1
-1 -1
-1 0
-1

2 2 2 1 1 1 1 1 1 1 1
1 -1
0 0
-1 0
-1 0
4

4 1 1 1 1 1 1 1 1 1 1
1 0 0 1
1

5 3 1 1 1 1 1 1 1 1 1
0 0 0 0 0
1 0 0 0 1
0 0 0 0 0
3

100 2 1 1 1 1 1 1 1 1 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1

99

5 3 2 1 1 1 1 1 1 1 1
0 -1 0 1 -1
1 -1 -1 -1 1
0 0 0 0 0
0 -1 1 -1 1
-1 1 -1 1 0
0 -1 0 0 0
3

5 5 3 2 1 1 1 1 1 1 1
1 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 0
0 0 0 0 1
5
'''