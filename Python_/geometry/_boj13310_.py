import sys
input = sys.stdin.readline


def cr(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0]-d1[0])*(d4[1]-d3[1]) - (d2[1]-d1[1])*(d4[0]-d3[0])


def cd(a1, a2):  # get c^2 = a^2 + b^2
    return (a1[0]-a2[0])**2 + (a1[1]-a2[1])**2


def mc(arr):  # get hull
    arr.sort(key=lambda x: (x[0], x[1]))
    if len(arr) <= 2:
        return arr
    lh = []
    for dl in arr:
        while len(lh) > 1 and cr(lh[-2], lh[-1], lh[-1], dl) <= 0:
            lh.pop()
        lh.append(dl)
    uh = []
    for du in reversed(arr):
        while len(uh) > 1 and cr(uh[-2], uh[-1], uh[-1], du) <= 0:
            uh.pop()
        uh.append(du)
    return lh[:-1] + uh[:-1]


def rc(h):  # get max distance of hull
    b = 1
    l = len(h)
    mx_d = 0
    for a in range(l + 1):
        while (b + 1) % l != a % l and cr(h[a % l], h[(a+1) % l], h[b % l], h[(b+1) % l]) > 0:
            if mx_d < cd(h[a % l], h[b % l]):
                mx_d = cd(h[a % l], h[b % l])
            b += 1
        if mx_d < cd(h[a % l], h[b % l]):
            mx_d = cd(h[a % l], h[b % l])
    return mx_d


def px(tp, x):  # compose list of positions at day x
    p_x = [(i[0] + i[2]*x, i[1] + i[3]*x) for i in tp]
    return p_x


def ts(d, x):  # get range of days during which the maximum length of the hull becomes smaller
    if x < 7:
        return (i for i in range(0, x + 1, 1))
    s, e = 0, x
    while e - s >= 3:
        l = (s*2 + e) // 3  # 1/3
        r = (s + e*2) // 3  # 2/3
        ml = rc(mc(px(d, l)))
        mr = rc(mc(px(d, r)))
        if ml > mr:
            s = l
        else:
            e = r
    return (i for i in range(s, e + 1, 1))


def gm(d, x):  # find min length of hulls and min day after ternary search
    ml = int(1e16)
    md = 0
    for i in x:
        mxi = rc(mc(px(d, i)))
        if ml > mxi:
            ml = mxi
            md = i
    return md, ml


n, t = map(int, input().split())
S = [tuple(map(int, input().split())) for _ in range(n)]
A = gm(S, ts(S, t))
print(A[0])
print(A[1])

'''
example

10 1000000
1 1 5 5
5 5 -1 1
2 3 2 3
4 3 5 6
6 5 4 3
3 3 -1 -1
6 9 -1 -2
7 8 -2 -4
0 0 10 0
-2 -3 -4 5

8 100
1 1 0 0
2 2 -1 0
3 4 1 0
5 4 -3 4
6 7 9 -1
0 3 3 3
2 0 -3 -3
1 0 2 1

10 100
50 60 -5 -5
50 50 -1 -1
20 30 -2 -3
40 30 -5 -6
-60 -50 4 3
-15 -30 1 1
60 90 -5 -6
70 80 -2 -4
0 0 7 0
-20 -30 4 5
'''
