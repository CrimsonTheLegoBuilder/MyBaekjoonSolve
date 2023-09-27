import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


N = int(sys.stdin.readline().strip())
hull = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
friends = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(3)]
yhj = [0, 0, 0]

for j in range(3):
    d_ = friends[j]
    x_, y_ = d_
    cnt = 0
    for i in range(N):
        d_1, d_2 = hull[i-1], hull[i]
        if d_1[1] > d_2[1]:
            d_1, d_2 = d_2, d_1
        x1, y1 = d_1
        x2, y2 = d_2
        if cross(d_1, d_, d_, d_2) == 0 and dot(d_1, d_, d_, d_2) >= 0:
            yhj[j] = 1
            break
        if x_ > max(x1, x2):
            continue
        if y_ <= min(y1, y2):
            continue
        if y_ > max(y1, y2):
            continue
        if cross(d_1, d_2, d_2, d_) < 0:
            continue
        if y_ == y1 == y2:
            continue
        if y1 < y_ <= y2:
            cnt += 1
    if cnt % 2:
        yhj[j] = 1

for i in yhj:
    print(i)

'''
5
0 0
5 0
5 5
4 2
0 20
-1 5
1 5
1 2

8
0 0
5 0
5 5
4 5
4 2
3 2
2 20
0 20
-1 5
1 5
1 2
'''