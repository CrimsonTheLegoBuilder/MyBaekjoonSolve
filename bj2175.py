import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def get_polygon_area(hull):  # get area
    pivot = (0, 0)
    a = len(hull)
    area = 0
    for i in range(a):
        area += cross(pivot, hull[i - 1], hull[i - 1], hull[i])
    return abs(area/2)


x1, y1, x3, y3, x5, y5, x7, y7 = map(float, sys.stdin.readline().strip().split())
x2, y2 = (x1+x3)/2, (y1+y3)/2
x4, y4 = (x3+x5)/2, (y3+y5)/2
x6, y6 = (x5+x7)/2, (y5+y7)/2
x8, y8 = (x7+x1)/2, (y7+y1)/2
dots = ((x1, y1), (x2, y2), (x3, y3), (x4, y4), (x5, y5), (x6, y6), (x7, y7), (x8, y8))
quad = get_polygon_area(dots)
# print(quad)
# print(dots)
ans = 12e12
for j in range(3, 8):
    for k in range(9-j):
        temp = get_polygon_area(dots[k:k+j])
        ans = min(ans, abs(2*temp-quad))
        # print(j, k, temp, ans, dots[k:k+j])
# print(ans)
A = (quad - ans)/2
B = (quad - ans)/2 + ans
print(A, B)
