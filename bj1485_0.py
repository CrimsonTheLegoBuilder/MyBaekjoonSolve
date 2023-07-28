import sys


def cross_3(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def monotone_chain(arr):  # get hull
    arr.sort(key=lambda x: (x[0], x[1]))
    if len(arr) <= 2:
        return arr
    lower = []
    for dl in arr:
        while len(lower) > 1 and cross_3(lower[-2], lower[-1], dl) <= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(arr):
        while len(upper) > 1 and cross_3(upper[-2], upper[-1], du) <= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


def cal_dist_square(a1, a2):  # get c^2 = a^2 + b^2
    return (a1[0] - a2[0])**2 + (a1[1] - a2[1])**2


n = int(sys.stdin.readline().strip())

for _ in range(n):
    square = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(4)]
    s = monotone_chain(square)
    # print(s)
    if len(s) == 4 and cross_3(s[-2], s[-1], s[0]) == cross_3(s[-1], s[0], s[1]) \
            == cross_3(s[0], s[1], s[2]) == cross_3(s[1], s[2], s[3]) and \
            cal_dist_square(s[0], s[1]) == abs(cross_3(s[0], s[1], s[2])):
        print(1)
    else:
        print(0)
