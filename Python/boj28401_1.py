import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


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


def is_cross(d1, d2, d3, d4):
    flag11 = cross(d1, d2, d2, d3)*cross(d2, d1, d1, d4)
    flag12 = cross(d3, d4, d4, d1)*cross(d4, d3, d3, d2)
    flag_1 = flag11 > 0 and flag12 > 0
    flag_2 = (cross(d1, d3, d3, d2) == 0 and dot(d1, d3, d3, d2) >= 0) |\
             (cross(d1, d4, d4, d2) == 0 and dot(d1, d4, d4, d2) >= 0) |\
             (cross(d3, d1, d1, d4) == 0 and dot(d3, d1, d1, d4) >= 0) |\
             (cross(d3, d2, d2, d4) == 0 and dot(d3, d2, d2, d4) >= 0)
    return flag_1 or flag_2


R, C, K = map(int, sys.stdin.readline().strip().split())
equip = []
for _ in range(K):
    r, c = map(int, sys.stdin.readline().strip().split())
    equip.append((c, r))
ds, de = (1, 1), (C, R)
dsu, deu, dsd, ded = (1, 2), (C, R+1), (2, 1), (C+1, R)

UC, DC = [dsu, deu], [dsd, ded]
for e in equip:
    x, y = e
    du, dd = (x+1, y), (x, y+1)
    i1, i2 = is_cross(dsu, deu, du, dd), is_cross(dsd, ded, du, dd)
    if i1 and i2:  # if completely blocked:
        print(0)
        exit(0)
    if i1:
        UC.append(du)
    if i2:
        DC.append(dd)
UH = monotone_chain(UC)
DH = monotone_chain(DC)
for i in range(len(UH)):
    for j in range(len(DH)):
        if is_cross(UH[i-1], UH[i], DH[j-1], DH[j]):
            print(0)
            exit(0)
print(1)
