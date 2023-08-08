import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def cal_dist_square(d1, d2):  # get c^2 = a^2 + b^2
    return (d1[0] - d2[0])**2 + (d1[1] - d2[1])**2


dots = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(3)]
segments = set()
for i in range(3):
    segments.add(cal_dist_square(dots[i-1], dots[i]))

angle = 1
for i in range(3):
    if dot(dots[i-2], dots[i-1], dots[i-1], dots[i]) == 0:
        angle = 0
    if dot(dots[i-2], dots[i-1], dots[i-1], dots[i]) > 0:
        angle = -1

if not cross(dots[0], dots[1], dots[1], dots[2]):
    print('X')
else:
    if len(segments) == 1:
        print('JungTriangle')
    if len(segments) == 2:
        if angle == 1:
            print('Yeahkak2Triangle')
        if angle == 0:
            print('Jikkak2Triangle')
        if angle == -1:
            print('Dunkak2Triangle')
    if len(segments) == 3:
        if angle == 1:
            print('YeahkakTriangle')
        if angle == 0:
            print('JikkakTriangle')
        if angle == -1:
            print('DunkakTriangle')


