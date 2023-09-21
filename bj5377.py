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
        while len(lower) > 1 and cross(lower[-2], lower[-1], lower[-1], dl) < 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(arr):
        while len(upper) > 1 and cross(upper[-2], upper[-1], upper[-1], du) < 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


def inner_check_strong(d1, arr):  # return True if dot inside convex polygon
    flag_i = 1
    for di in range(len(arr)):
        if cross(arr[di-1], arr[di], arr[di], d1) <= 0:
            flag_i = 0
    return flag_i


def is_cross(d1, d2, d3, d4):
    flag11 = cross(d1, d2, d2, d3)*cross(d2, d1, d1, d4)
    flag12 = cross(d3, d4, d4, d1)*cross(d4, d3, d3, d2)
    flag_1 = flag11 > 0 and flag12 > 0
    flag_2 = (cross(d1, d3, d3, d2) == 0 and dot(d1, d3, d3, d2) >= 0) |\
             (cross(d1, d4, d4, d2) == 0 and dot(d1, d4, d4, d2) >= 0) |\
             (cross(d3, d1, d1, d4) == 0 and dot(d3, d1, d1, d4) >= 0) |\
             (cross(d3, d2, d2, d4) == 0 and dot(d3, d2, d2, d4) >= 0)
    return flag_1 or flag_2


N = int(sys.stdin.readline().strip())
for _ in range(N):
    ds = tuple(map(int, sys.stdin.readline().strip().split()))
    de = tuple(map(int, sys.stdin.readline().strip().split()))
    M = int(sys.stdin.readline().strip())
    students = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(M)]
    hull = monotone_chain(students)
    flag1 = inner_check_strong(ds, hull)
    flag2 = inner_check_strong(de, hull)
    if flag1 or flag2:
        print("IMPOSSIBLE")
    else:
        hull += [ds]
        hull = monotone_chain(hull)
        flag = 1
        for i in range(len(hull)):
            if hull[i-1] == ds or hull[i] == ds:
                continue
            if is_cross(ds, de, hull[i-1], hull[i]):
                flag = 0
        if flag:
            print(f"{cal_dist(ds, de):.3f}")
        else:
            hull += [de]
            hull = monotone_chain(hull)
            L = len(hull)
            round_hull = 0.0
            for i in range(L):
                round_hull += cal_dist(hull[i-1], hull[i])
            start = 0
            for i in range(L):
                if hull[i] == ds:
                    start = i
                    break
            ans = 0.0
            for j in range(start, start+L):
                if hull[j % L] == de:
                    break
                ans += cal_dist(hull[j % L], hull[(j+1) % L])
            ans = min(ans, round_hull-ans)
            print(f"{ans:.3f}")

'''
12
0 0
4 0
4
1 1
2 2
3 1
2 -5
0 0
5 0
4
0 -5
0 5
5 -5
5 5
0 0
3 -3
4
1 -1
2 -2
3 -1
2 5
3 -3
0 0
4
1 -1
2 -2
3 -1
2 5
0 0
6 0
4
0 -5
0 5
5 -5
5 5
6 0
0 0
4
0 -5
0 5
5 -5
5 5
-1 0
5 0
3
0 0
5 5
5 -5
5 0
11 0
3
10 0
5 5
5 -5
0 1
0 -1
6
1 1
2 0
1 -1
-1 -1
-2 0
-1 1
0 0
3 3
4
1 1
1 2
2 2
2 1
1 1
1 2
2
0 0
2 0
0 0
3 3
1
1 1
'''