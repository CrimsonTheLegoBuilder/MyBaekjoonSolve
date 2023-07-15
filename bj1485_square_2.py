import sys


def dot(arr1, arr2):
    ans = arr1[0] * arr2[0] + arr1[1] * arr2[1]
    return ans


n = int(sys.stdin.readline().strip())

for _ in range(n):
    dot1 = list(map(int, sys.stdin.readline().strip().split()))
    dot2 = list(map(int, sys.stdin.readline().strip().split()))
    dot3 = list(map(int, sys.stdin.readline().strip().split()))
    dot4 = list(map(int, sys.stdin.readline().strip().split()))
    v1 = [dot2[0] - dot1[0], dot2[1] - dot1[1]]
    v2 = [dot3[0] - dot1[0], dot3[1] - dot1[1]]
    v3 = [dot4[0] - dot1[0], dot4[1] - dot1[1]]
    l1 = v1[0] ** 2 + v1[1] ** 2
    l2 = v2[0] ** 2 + v2[1] ** 2
    l3 = v3[0] ** 2 + v3[1] ** 2

    if (l1 == l2) & (l2 * 2 == l3) & (dot(v1, v3) > 0) & (dot(v2, v3) > 0):
        sys.stdout.write('1\n')
    elif (l2 == l3) & (l2 * 2 == l1) & (dot(v2, v1) > 0) & (dot(v3, v1) > 0):
        sys.stdout.write('1\n')
    elif (l1 == l3) & (l1 * 2 == l2) & (dot(v1, v2) > 0) & (dot(v3, v2) > 0):
        sys.stdout.write('1\n')
    else:
        sys.stdout.write('0\n')
