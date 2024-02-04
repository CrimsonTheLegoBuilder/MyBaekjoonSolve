import sys


def quad(r1, r2, c1, c2):
    if r1 == r2 and c1 == c2:
        if arr[r1][c1] == 1:
            return 0, 1
        else:
            return 1, 0
    mid1 = (r1 + r2) // 2
    mid2 = (c1 + c2) // 2
    lu = quad(r1, mid1, c1, mid2)
    ld = quad(mid1 + 1, r2, c1, mid2)
    ru = quad(r1, mid1, mid2 + 1, c2)
    rd = quad(mid1 + 1, r2, mid2 + 1, c2)

    result = (lu[0] + ld[0] + ru[0] + rd[0], lu[1] + ld[1] + ru[1] + rd[1])
    if result == (4, 0):
        return 1, 0
    if result == (0, 4):
        return 0, 1
    return result


N = int(sys.stdin.readline().strip())
arr = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
a, b = quad(0, N - 1, 0, N - 1)
print(a)
print(b)
