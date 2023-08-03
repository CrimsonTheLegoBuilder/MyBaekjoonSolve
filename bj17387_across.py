import sys

x1, y1, x2, y2 = map(int, sys.stdin.readline().strip().split())
x3, y3, x4, y4 = map(int, sys.stdin.readline().strip().split())
dot1 = [x1, y1]
dot2 = [x2, y2]
dot3 = [x3, y3]
dot4 = [x4, y4]


def cross(arr1, arr2, x):
    ans = (arr1[0] - x[0]) * (arr2[1] - arr1[1]) - (arr2[0] - arr1[0]) * (arr1[1] - x[1])
    return ans


def dot(d1, d2, d3):  # dots product / get projection / get force / for 3 dots
    return (d2[0] - d1[0]) * (d3[0] - d2[0]) + (d2[1] - d1[1]) * (d3[1] - d2[1])


crs1 = cross(dot1, dot2, dot3)
crs2 = cross(dot2, dot1, dot4)
crs3 = cross(dot3, dot4, dot1)
crs4 = cross(dot4, dot3, dot2)
dt1 = dot(dot4, dot1, dot3)
dt2 = dot(dot3, dot2, dot4)
dt3 = dot(dot1, dot3, dot2)
dt4 = dot(dot2, dot4, dot1)

if (dt3 > 0 and crs1 == 0) | (dt4 > 0 and crs2 == 0) | (dt1 > 0 and crs3 == 0) | (dt2 > 0 and crs4 == 0):
    print(1)
elif dot1 == dot3 or dot1 == dot4 or dot2 == dot3 or dot2 == dot4:
    print(1)
elif (crs1 * crs2 > 0) & (crs3 * crs4 > 0):
    print(1)
else:
    print(0)
