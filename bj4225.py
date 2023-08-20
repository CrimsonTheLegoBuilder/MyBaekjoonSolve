import sys
from math import ceil


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def monotone_chain(arr):  # get hull / sorting hull CCW
    arr.sort(key=lambda x: (x[0], x[1]))
    if len(arr) <= 2:
        return arr
    lower = []
    for dl in arr:
        while len(lower) > 1 and cross(lower[-2], lower[-1], lower[-1], dl) <= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(arr):
        while len(upper) > 1 and cross(upper[-2], upper[-1], upper[-1], du) <= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


def line_to_dot(d1, d2, d3):
    return abs(cross(d1, d2, d2, d3)) / cal_dist(d1, d2)


tc = 0
while True:
    tc += 1
    N = int(sys.stdin.readline().strip())
    if not N:
        break
    dots = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
    trash = monotone_chain(dots)
    ans = 12e12
    l = len(trash)
    for i in range(l):
        for j in range(l):
            if j == i or j == (i+1) % l:
                continue
            D = line_to_dot(trash[i], trash[(i+1) % l], trash[j])
            ans = min(ans, D)
    # print(f'Case {tc}: {ans + 0.005:.2f}')
    print(f'Case {tc}: {ceil(ans * 100)/100:.2f}')
    # print(f'Case {tc}: {1.50:.2f}')
    # print(f'Case {tc}: {ceil(1.5000001*100)/100:.2f}')
