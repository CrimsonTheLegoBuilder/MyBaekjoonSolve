import sys


def quad(r1, r2, c1, c2):
    if r1 == r2 and c1 == c2:
        if arr[r1][c1] == '1':
            return '1'
        else:
            return '0'
    mid1 = (r1 + r2) // 2
    mid2 = (c1 + c2) // 2
    lu = quad(r1, mid1, c1, mid2)
    ld = quad(mid1 + 1, r2, c1, mid2)
    ru = quad(r1, mid1, mid2 + 1, c2)
    rd = quad(mid1 + 1, r2, mid2 + 1, c2)

    result = '(' + lu + ru + ld + rd + ')'

    if result == '(1111)':
        return '1'
    if result == '(0000)':
        return '0'
    return result


N = int(sys.stdin.readline().strip())
arr = [list(str(sys.stdin.readline().strip())) for _ in range(N)]
a = quad(0, N - 1, 0, N - 1)
print(a)
