import sys
from functools import cmp_to_key


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def cmp_c(d1, d2):
    return -cross(p, d1, d1, d2)


for _ in range(int(sys.stdin.readline().strip())):
    temp = list(map(int, sys.stdin.readline().strip().split()))
    dots = []
    for i in range(1, temp[0]+1):
        dots.append((temp[i*2-1], temp[i*2], i-1))
    # print(dots)
    dots.sort(key=lambda x: (x[0], x[1]))
    p = dots[0]
    hull = [p] + sorted(dots[1:], key=cmp_to_key(cmp_c))
    k = -1
    while 1:
        if not cross(hull[k+1], hull[k], hull[k], hull[k-1]):
            k -= 1
        else:
            break
    ans = hull[:k] + hull[:k-1:-1]
    for _, _, d in ans[:-1]:
        print(d, end=' ')
    print(ans[-1][-1])
