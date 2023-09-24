import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


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
    equip.append((2*c, 2*r))
ds, de = (2, 2), (2*C, 2*R)
dsu, deu, dsd, ded = (1, 3), (2*C-1, 2*R+1), (3, 1), (2*C+1, 2*R-1)
dsu_, deu_, dsd_, ded_ = (0, 4), (2*C-2, 2*R+2), (4, 0), (2*C+2, 2*R-2)
U, D = [], []
for e in equip:
    if not cross(ds, de, de, e):  # if completely blocked:
        print(0)
        exit(0)
    if cross(ds, de, de, e) < 0 <= cross(dsd_, ded_, ded_, e):
        D.append(e)
    if cross(ds, de, de, e) > 0 >= cross(dsu_, deu_, deu_, e):
        U.append(e)
# print(U, D)
UC, DC = [dsu, deu], [dsd, ded]
for x, y in U:
    UC += [(x+1, y-1)]
for x, y in D:
    DC += [(x-1, y+1)]
UH = monotone_chain(UC)
DH = monotone_chain(DC)
# print(UH, DH)
for i in range(len(UH)):
    for j in range(len(DH)):
        if is_cross(UH[i-1], UH[i], DH[j-1], DH[j]):
            print(0)
            exit(0)
print(1)
