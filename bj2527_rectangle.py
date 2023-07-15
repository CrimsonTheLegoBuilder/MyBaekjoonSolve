import sys

for _ in range(4):
    q, w, e, r, t, y, u, i = map(int, sys.stdin.readline().strip().split())  # 변수 할당 귀찮아
    rec1 = [[q, w], [q, r], [e, r], [e, w]]
    rec2 = [[t, y], [t, i], [u, i], [u, y]]
    cnt_c = 0
    for k in rec1:
        for j in rec2:
            if k == j:
                cnt_c += 1
    if ((q < t < e) & (w < y < r)) | ((q < t < e) & (w < i < r)) | ((q < u < e) & (w < i < r)) | ((q < u < e) & (w < y < r)) | \
            ((q < t < u < e) & (y < w < r < i)):
        sys.stdout.write('a\n')
    else:
        if cnt_c == 1:
            sys.stdout.write('c\n')
        elif (q == t) | (q == u) | (e == t) | (e == u) | (w == y) | (w == i) | (r == y) | (r == i):
            sys.stdout.write('b\n')
        else:
            sys.stdout.write('d\n')
