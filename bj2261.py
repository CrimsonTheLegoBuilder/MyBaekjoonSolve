import sys


def cal_dist_square(d1, d2):  # get c^2 = a^2 + b^2
    return (d1[0] - d2[0])**2 + (d1[1] - d2[1])**2


def divide_and_conquer(s, e, arr):
    l_ = s - e
    if l_ <= 3:
        return min_dist(s, e, arr)
    mid = l_ // 2
    left = divide_and_conquer(s, mid, arr)
    right = divide_and_conquer(mid, e, arr)
    min_2 = min(left, right)
    dist = min_dist_2(s, e, mid, min_2, arr)
    return min(min_2, dist)


def min_dist(s, e, arr):
    dist = int(12e12)
    for i in range(s, e - 1):
        for j in range(i+1, e):
            temp = cal_dist_square(arr[i], arr[j])
            if dist > temp:
                dist = temp
    return dist


def min_dist_2(s, e, mid, dist, arr):
    band = []
    for i in range(s, e):
        x = arr[i][0]
        if (x - arr[mid][0])**2 < dist:
            band.append(arr[i])
    min_dist_ = dist
    band.sort(key=lambda y: y[1])
    for i in range(len(band)-1):
        for j in range(i+1, i+7):
            if j >= len(band) or cal_dist_square(band[i], band[j]) > min_dist_:
                break
            min_dist_ = min(cal_dist_square(band[i], band[j]), min_dist_)
    return min_dist_


N = int(sys.stdin.readline().strip())

dots = sorted([tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)])
ans = divide_and_conquer(0, N, dots)
print(ans)
