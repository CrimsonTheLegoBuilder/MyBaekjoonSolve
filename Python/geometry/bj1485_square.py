import sys


def cross(arr1, arr2):
    ans = arr1[0] * arr2[1] - arr1[1] * arr2[0]
    return ans


n = int(sys.stdin.readline().strip())

for _ in range(n):
    dot1 = list(map(int, sys.stdin.readline().strip().split()))
    dot2 = list(map(int, sys.stdin.readline().strip().split()))
    dot3 = list(map(int, sys.stdin.readline().strip().split()))
    dot4 = list(map(int, sys.stdin.readline().strip().split()))
    v1 = [dot1[0] - dot2[0], dot1[1] - dot2[1]]
    v2 = [dot3[0] - dot4[0], dot3[1] - dot4[1]]
    v3 = [dot1[0] - dot3[0], dot1[1] - dot3[1]]
    v4 = [dot2[0] - dot4[0], dot2[1] - dot4[1]]
    l1 = (v1[0] ** 2 + v1[1] ** 2) ** 2
    l2 = (v2[0] ** 2 + v2[1] ** 2) ** 2
    l3 = (v3[0] ** 2 + v3[1] ** 2) ** 2
    l4 = (v4[0] ** 2 + v4[1] ** 2) ** 2
    dot1 = v1[0] * v2[0] + v1[1] * v2[1]
    dot2 = v3[0] * v4[0] + v3[1] * v4[1]
    C1 = cross(v1, v2)
    C2 = cross(v3, v4)
    if ((abs(C1) == l1 * l2) & (l1 == l2) & (-1 * dot2 == l3 * l4)) | ((abs(C2) == l3 * l4) & (l3 == l4) & (-1 * dot1 == l1 * l2)):
        sys.stdout.write('1\n')
    elif (dot1 == l1 * l2) & (dot2 == l3 * l4) & (abs(cross(v1, v3)) == l1 * l3):
        sys.stdout.write('0\n')
    else:
        sys.stdout.write('0\n')
