import sys


def cal_dist_square(d1, d2):  # get c^2 = a^2 + b^2
    return (d1[0] - d2[0])**2 + (d1[1] - d2[1])**2


def divide_and_conquer(s, e):
    l_ = e - s
    if l_ <= 2:
        return min_dist(s, e)
    mid = (s+e) // 2
    left = divide_and_conquer(s, mid)
    right = divide_and_conquer(mid, e)
    min_2 = min(left, right)
    dist = min_dist_2(s, e, mid, min_2)
    return min(min_2, dist)


def min_dist(s, e):
    dist = int(12e12)
    for i in range(s, e):
        for j in range(i+1, e):
            temp = cal_dist_square(dots[i], dots[j])
            if dist > temp:
                dist = temp
    return dist


def min_dist_2(s, e, mid, dist):
    band = []
    for i in range(s, e):
        x = dots[i][0]
        if (x - dots[mid][0])**2 < dist:
            band.append(dots[i])
    min_dist_ = dist
    band.sort(key=lambda y: y[1])
    for i in range(len(band)-1):
        for j in range(i+1, len(band)):
            if (band[i][1] - band[j][1])**2 < min_dist_:
                min_dist_ = min(cal_dist_square(band[i], band[j]), min_dist_)
            else:
                break
    return min_dist_


N = int(sys.stdin.readline().strip())

dots = sorted([tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)])
ans = divide_and_conquer(0, N)
print(ans)
