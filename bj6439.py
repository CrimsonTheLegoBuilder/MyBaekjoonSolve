import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def cross_s(s1, s2):  # s_ = (x1, y1, x2, y2)
    result1 = (s1[2] - s1[0]) * (s2[1] - s1[3]) - (s1[3] - s1[1]) * (s2[0] - s1[2])
    result2 = (s1[0] - s1[2]) * (s2[3] - s1[1]) - (s1[1] - s1[3]) * (s2[2] - s1[0])
    result3 = (s2[2] - s2[0]) * (s1[1] - s2[3]) - (s2[3] - s2[1]) * (s1[0] - s2[2])
    result4 = (s2[0] - s2[2]) * (s1[3] - s2[1]) - (s2[1] - s2[3]) * (s1[2] - s2[0])
    flag1 = (result1 * result2 > 0) * (result3 * result4 > 0)
    if result1 == 0 and (s2[0] - s1[0]) * (s1[2] - s2[0]) + (s2[1] - s1[1]) * (s1[3] - s2[1]) >= 0:
        flag1 = 1
    if result2 == 0 and (s2[2] - s1[0]) * (s1[2] - s2[2]) + (s2[3] - s1[1]) * (s1[3] - s2[3]) >= 0:
        flag1 = 1
    if result3 == 0 and (s1[0] - s2[0]) * (s2[2] - s1[0]) + (s1[1] - s2[1]) * (s2[3] - s1[1]) >= 0:
        flag1 = 1
    if result4 == 0 and (s1[2] - s2[0]) * (s2[2] - s1[2]) + (s1[3] - s2[1]) * (s2[3] - s1[3]) >= 0:
        flag1 = 1
    return flag1


N = int(sys.stdin.readline().strip())
xs, ys, xe, ye, x1, y1, x2, y2 = map(int, sys.stdin.readline().strip().split())
line = (xs, ys, xe, ye)
if x1 > x2:
    x1, x2 = x2, x1
if y1 > y2:
    y1, y2 = y2, y1
d_1, d_2, d_3, d_4 = (x1, y1), (x2, y1), (x2, y2), (x1, y2)
hull = (d_1, d_2, d_3, d_4)
ans = 'F'
for i in range(4):
    if cross_s(line, (*hull[i], *hull[(i+1) % 4])):
        ans = 'T'
print(ans)
