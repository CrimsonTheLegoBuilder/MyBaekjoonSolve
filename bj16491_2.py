import sys


def cross(d1, d2, d3, d4):
    return (d2[0]-d1[0])*(d4[1]-d3[1]) - (d2[1]-d1[1])*(d4[0]-d3[0])


def dot(d1, d2, d3, d4):
    return (d2[0]-d1[0])*(d4[0]-d3[0]) + (d2[1]-d1[1])*(d4[1]-d3[1])


def cross_check(d1, d2, d3, d4):
    flag1 = cross(d1, d2, d2, d3) * cross(d2, d1, d1, d4)
    flag2 = cross(d3, d4, d4, d1) * cross(d4, d3, d3, d2)
    flag3 = (not cross(d1, d2, d2, d3) and dot(d1, d3, d3, d2) > 0) or \
            (not cross(d1, d2, d2, d4) and dot(d1, d4, d4, d2) > 0)
    flag4 = (not cross(d3, d4, d4, d1) and dot(d3, d1, d1, d4) > 0) or \
            (not cross(d3, d4, d4, d2) and dot(d3, d2, d2, d4) > 0)
    return (flag1 > 0 and flag2 > 0) or flag3 or flag4


def dfs(x, v):
    if x == N:
        for j in range(N):
            print(perm[j]+1)
        exit(0)
    for i in range(N):
        if not v & 1 << i:
            d_1, d_2 = robots[x], shelter[i]
            c = 1
            for k in range(0, x):
                d_3, d_4 = robots[k], shelter[perm[k]]
                if cross_check(d_1, d_2, d_3, d_4):
                    c = 0
                    break
            if c:
                perm[x] = i
                v |= 1 << i
                dfs(x+1, v)
                v ^= 1 << i


N = int(sys.stdin.readline().strip())
robots = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
shelter = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
V = 0
perm = [0]*N
dfs(0, V)
print('Impossible')