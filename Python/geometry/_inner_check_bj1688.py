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


def inner_check(d, arr):
    flag = 1
    for x in range(len(arr)):
        if cross(d, arr[x - 1], arr[x - 1], arr[x]) < 0:
            flag = 0
        if cross(d, arr[x - 1], arr[x - 1], arr[x]) == 0 and dot(arr[x - 1], d, d, arr[x]) < 0:
            flag = 0
    return flag


n = int(sys.stdin.readline().strip())
dots = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
hull = monotone_chain(dots)
friends = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(3)]
for i in friends:
    print(inner_check(i, hull))
