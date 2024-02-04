import sys


def cross(d1, d2, d3, d4):
    return (d4[1] - d3[1]) * (d2[0] - d1[0]) - (d4[0] - d3[0]) * (d2[1] - d1[1])


def monotone_chain(hull):
    hull.sort(key=lambda d: (d[0], d[1]))
    if len(hull) <= 2:
        return hull
    lower = []
    for o in hull:
        while len(lower) > 1 and cross(lower[-2], lower[-1], lower[-1], o) <= 0:
            lower.pop()
        lower.append(o)
    lower.pop()
    upper = []
    for o in range(len(hull)-1, -1, -1):
        while len(upper) > 1 and cross(upper[-2], upper[-1], upper[-1], hull[o]) <= 0:
            upper.pop()
        upper.append(hull[o])
    upper.pop()
    return lower + upper


N, M = map(int, sys.stdin.readline().strip().split())
N_ = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
M_ = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(M)]
hull_cnd = set()
for i in N_:
    for j in M_:
        temp = (i[0]+j[0], i[1]+j[1])
        hull_cnd.add(temp)

hull_cnd = list(hull_cnd)
ans = monotone_chain(hull_cnd)
print(len(ans))
for i in ans:
    print(*i)
