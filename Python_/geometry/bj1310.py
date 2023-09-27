import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def cal_dist_square(d1, d2):  # get c^2 = a^2 + b^2
    return (d1[0] - d2[0])**2 + (d1[1] - d2[1])**2


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


def rotating_calipers(hull):  # get the largest distance of hull
    fl2 = 1
    l = len(hull)
    max_dist = 0
    for i_ in range(l + 1):
        while (fl2 + 1) % l != i_ % l and cross(hull[i_ % l], hull[(i_ + 1) % l], hull[fl2 % l], hull[(fl2 + 1) % l]) > 0:
            if max_dist < cal_dist_square(hull[i_ % l], hull[fl2 % l]):
                max_dist = cal_dist_square(hull[i_ % l], hull[fl2 % l])
            fl2 += 1
        if max_dist < cal_dist_square(hull[i_ % l], hull[fl2 % l]):
            max_dist = cal_dist_square(hull[i_ % l], hull[fl2 % l])
    return max_dist


dots = []
for _ in range(int(sys.stdin.readline().strip())):
    dots.append(tuple(map(int, sys.stdin.readline().strip().split())))
hull_ground = monotone_chain(dots)
ans = rotating_calipers(hull_ground)
print(ans)
