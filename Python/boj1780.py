import sys


def cnt(u, d, l, r):
    if r == l:
        if P[u][l] == -1:
            return 1, 0, 0
        elif P[u][l] == 1:
            return 0, 0, 1
        else:
            return 0, 1, 0
    ru, rd = (u*2+d)//3, (u+2*d)//3
    cl, cr = (l*2+r)//3, (l+2*r)//3
    p1 = cnt(u, ru, l, cl)
    p2 = cnt(u, ru, cl+1, cr)
    p3 = cnt(u, ru, cr+1, r)
    p4 = cnt(ru+1, rd, l, cl)
    p5 = cnt(ru+1, rd, cl+1, cr)
    p6 = cnt(ru+1, rd, cr+1, r)
    p7 = cnt(rd+1, d, l, cl)
    p8 = cnt(rd+1, d, cl+1, cr)
    p9 = cnt(rd+1, d, cr+1, r)
    pp = (
        p1[0] + p2[0] + p3[0] + p4[0] + p5[0] + p6[0] + p7[0] + p8[0] + p9[0],
        p1[1] + p2[1] + p3[1] + p4[1] + p5[1] + p6[1] + p7[1] + p8[1] + p9[1],
        p1[2] + p2[2] + p3[2] + p4[2] + p5[2] + p6[2] + p7[2] + p8[2] + p9[2]
    )
    if pp[0] == pp[1] == 0:
        return 0, 0, 1
    if pp[0] == pp[2] == 0:
        return 0, 1, 0
    if pp[1] == pp[2] == 0:
        return 1, 0, 0
    return pp


n = int(sys.stdin.readline().strip())
P = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
p = cnt(0, n-1, 0, n-1)
print(f"{p[0]}\n{p[1]}\n{p[2]}")


'''
import sys
input = sys.stdin.readline
cnt = [0, 0, 0]

N = int(input())
paper = [list(map(int, input().split())) for _ in range(N)]


def conquer(x, y, n):
    lst = [paper[i][y:y+n] for i in range(x, x+n)]
    if len(set(k := sum(lst, []))) == 1:
        cnt[k[0]] += 1
        return

    d = n // 3
    for i in range(0, n, d):
        for j in range(0, n, d):
            conquer(x+i, x+j, d)


conquer(0, 0, N)
print(cnt[-1])
print(cnt[0])
print(cnt[1])
'''