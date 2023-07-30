import sys


def cross_3(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


for _ in range(4):
    x1, y1, x2, y2, x3, y3, x4, y4 = map(int, sys.stdin.readline().strip().split())
    rec1 = [(x1, y1), (x2, y1), (x2, y2), (x1, y2)]
    rec2 = [(x3, y3), (x4, y3), (x4, y4), (x3, x4)]
    dots = set(rec1+rec2)
    cnt_a = 0
    cnt_b = 0
    cnt_c = 0
    for k in rec1:
        for j in rec2:
            if k == j:
                cnt_c += 1
    if cross_3(rec1[0], rec1[1], rec2[0]) * cross_3(rec1[1], rec1[0], rec2[3]) > 0 and \
            cross_3(rec2[0], rec2[3], rec1[0]) * cross_3(rec2[3], rec2[0], rec1[1]) > 0:
        cnt_a += 1
    if cross_3(rec1[0], rec1[3], rec2[0]) * cross_3(rec1[3], rec1[0], rec2[1]) > 0 and \
            cross_3(rec2[0], rec2[1], rec1[0]) * cross_3(rec2[1], rec2[0], rec1[3]) > 0:
        cnt_a += 1
    for a in range(4):
        for b in range(4):
            if cross_3(rec1[a], rec2[b-1], rec2[b]) > 0:
                cnt_a += 1
            if cross_3(rec2[b], rec1[a-1], rec1[a]) > 0:
                cnt_a += 1


