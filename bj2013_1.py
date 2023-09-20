import sys
sys.stdin = open("bj2013_input.txt", "r")


def cross(d1, d2, d3, d4):
    return (d2[0]-d1[0])*(d4[1]-d3[1]) - (d2[1]-d1[1])*(d4[0]-d3[0])


def dot(d1, d2, d3, d4):
    return (d2[0]-d1[0])*(d4[0]-d3[0]) + (d2[1]-d1[1])*(d4[1]-d3[1])


def find_set(v):
    if L[v][0] == v:
        return v
    L[v][0] = find_set(L[v][0])
    return L[v][0]


def union(v1, v2):
    v1 = find_set(v1)
    v2 = find_set(v2)
    if v1 == v2:
        return
    _, x1, y1, x2, y2 = L[v1]
    _, x3, y3, x4, y4 = L[v2]
    d1, d2, d3, d4 = (x1, y1), (x2, y2), (x3, y3), (x4, y4)
    flag1 = cross(d1, d2, d3, d4)
    flag2 = (dot(d1, d3, d3, d2) >= 0) or (dot(d1, d4, d4, d2) >= 0) or (dot(d3, d1, d1, d4) >= 0) or (dot(d3, d2, d2, d4) >= 0)
    if flag1 == 0 and flag2:
        # print(2)
        d5 = d1 if d1 < d3 else d3
        d6 = d2 if d2 > d4 else d4
        if v1 < v2:
            L[v2][0] = v1
            L[v1] = [v1, *d5, *d6]
        else:
            L[v1][0] = v2
            L[v2] = [v2, *d5, *d6]
        # print(L)


def divide(left, right):
    if left < right:
        m = (right + left) // 2
        divide(left, m)
        divide(m+1, right)
        conquer(left, right)


def conquer(left, right):
    m = (right + left) // 2
    for x in range(left, m+1):
        for y in range(m+1, right+1):
            # print(x, y)
            union(x, y)


while 1:
    N = int(sys.stdin.readline().strip())
    # print(N)
    if not N:
        break
    L = [[i, 0, 0, 0, 0] for i in range(N)]
    for i in range(N):
        x_1, y_1, x_2, y_2 = map(lambda x: int(x * 100), map(float, sys.stdin.readline().strip().split()))
        # x_1, y_1, x_2, y_2 = map(float, sys.stdin.readline().strip().split())
        # if (x_1, y_1) > (x_2, y_2):
        if x_1 > x_2 or x_1 == x_2 and y_1 > y_2:
            x_1, y_1, x_2, y_2 = x_2, y_2, x_1, y_1
        L[i][1], L[i][2], L[i][3], L[i][4] = x_1, y_1, x_2, y_2

    divide(0, N-1)
    ans = set()
    for i in range(N):
        ans.add(L[i][0])
    print(len(ans))
