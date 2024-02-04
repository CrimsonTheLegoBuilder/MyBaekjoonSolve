import sys


def cross(d1, d2, d3):  # cross product / CCW / get area / for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def cal_area(arr):
    area = 0
    pivot = (0, 0)
    for i in range(len(arr)):
        area += cross(pivot, arr[i - 1], arr[i])
    return abs(area/2)


def intersection(arr1, arr2):
    den = arr1[0] * arr2[1] - arr1[1] * arr2[0]
    if den == 0:
        return None
    x, y = None, None
    if arr1[0] == 0:
        x = (arr1[2] * arr2[1] - arr1[1] * arr2[2]) / (arr1[1] * arr2[0])
        y = -(arr1[2] / arr1[1])
    elif arr1[1] == 0:
        x = -(arr1[2] / arr1[0])
        y = (arr1[2] * arr2[0] - arr1[0] * arr2[2]) / (arr1[0] * arr2[1])
    elif arr2[0] == 0:
        x = (arr1[1] * arr2[2] - arr1[2] * arr2[1]) / (arr1[0] * arr2[1])
        y = -(arr2[2] / arr2[1])
    elif arr2[1] == 0:
        x = -(arr2[2] / arr2[0])
        y = (arr1[0] * arr2[2] - arr1[2] * arr2[0]) / (arr1[1] * arr2[0])
    elif arr1[0] != 0 and arr1[1] != 0 and arr2[0] != 0 and arr2[1] != 0:
        x = ((arr1[2] * arr2[0]) - (arr1[0] * arr2[2])) / den
        y = ((arr1[2] * arr2[1]) - (arr1[1] * arr2[2])) / den
    return x, y


t = int(sys.stdin.readline().strip())
for _ in range(t):
    lines = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(3)]
    dots = [intersection(lines[i], lines[(i + 1) % 3]) for i in range(3) if intersection(lines[i], lines[(i + 1) % 3])]
    if len(dots) <= 2:
        print('0.0000')
        continue
    # print(dots)
    ans = cal_area(dots)
    print(f'{ans:.4f}')
    # print(f'{round(ans, 4):.4f}')
