import sys


def cross(d1, d2, d3, d4):
    return (d2[0]-d1[0])*(d4[1]-d3[1]) - (d2[1]-d1[1])*(d4[0]-d3[0])


def dot(d1, d2, d3, d4):
    return (d2[0]-d1[0])*(d4[0]-d3[0]) + (d2[1]-d1[1])*(d4[1]-d3[1])


def is_cross(d1, d2, d3, d4):
    flag11 = cross(d1, d2, d2, d3)*cross(d2, d1, d1, d4)
    flag12 = cross(d3, d4, d4, d1)*cross(d4, d3, d3, d2)
    flag1 = flag11 > 0 and flag12 > 0
    flag2 = (cross(d1, d3, d3, d2) == 0 and dot(d1, d3, d3, d2) >= 0) |\
            (cross(d1, d4, d4, d2) == 0 and dot(d1, d4, d4, d2) >= 0) |\
            (cross(d3, d1, d1, d4) == 0 and dot(d3, d1, d1, d4) >= 0) |\
            (cross(d3, d2, d2, d4) == 0 and dot(d3, d2, d2, d4) >= 0)
    return flag1 or flag2


def find_set(v):
    if P[v] == v:
        return v
    P[v] = find_set(P[v])
    return P[v]


def union(v1, v2):
    v1 = find_set(v1)
    v2 = find_set(v2)
    if v1 == v2:
        return
    if v1 < v2:
        P[v2] = v1
    else:
        P[v1] = v2


N = int(sys.stdin.readline().strip())
P = [i for i in range(N)]
dots = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
for x in range(N-1):
    for y in range(x+1, N):
        x_1, y_1, x_2, y_2 = dots[x]
        x_3, y_3, x_4, y_4 = dots[y]
        if is_cross((x_1, y_1), (x_2, y_2), (x_3, y_3), (x_4, y_4)):
            union(x, y)

for i in range(N):
    find_set(i)
ans = {}
size = 0
for i in range(N):
    ans.setdefault(P[i], 0)
    ans[P[i]] += 1
    size = max(size, ans[P[i]])
print(len(ans))
print(size)
