import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def get_polygon_area(hull):  # get area
    pivot = (0, 0)
    a = len(hull)
    area = 0
    for d in range(a):
        area += cross(pivot, hull[d - 1], hull[d - 1], hull[d])
    return abs(area/2)


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


def get_new_arr(arr, o):
    if o == 'L':
        arr.sort(key=lambda x: -x[0])
    elif o == 'R':
        arr.sort(key=lambda x: x[0])
    elif o == 'U':
        arr.sort(key=lambda y: y[1])
    elif o == 'D':
        arr.sort(key=lambda y: -y[1])
    arr.pop()


N = int(sys.stdin.readline().strip())
nails = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
order = str(sys.stdin.readline().strip())
hull_nail = monotone_chain(nails)
ans = get_polygon_area(hull_nail)
print(ans)
for i in order[:-1]:
    get_new_arr(nails, i)
    # print(nails)
    hull_nail = monotone_chain(nails)
    ans = get_polygon_area(hull_nail)
    print(ans)


'''
'L': 가장 왼쪽 못 (가장 작은 x좌표)
'R': 가장 오른쪽 못 (가장 큰 x좌표)
'U': 가장 위쪽 못 (가장 큰 y좌표)
'D': 가장 아래쪽 못 (가장 작은 y좌표)
'''