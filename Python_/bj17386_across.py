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


crs1 = cross(dot1, dot2, dot3)
crs2 = cross(dot2, dot1, dot4)
crs3 = cross(dot3, dot4, dot1)
crs4 = cross(dot4, dot3, dot2)

# if (crs1 == 0) | (crs2 == 0) | (crs3 == 0) | (crs4 == 0):
#     print(1)
if (crs1 * crs2 > 0) & (crs3 * crs4 > 0):
    sys.stdout.write('1\n')
else:
    sys.stdout.write('0\n')
