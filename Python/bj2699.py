import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def monotone_chain_r(arr):  # reversed / get hull / sorting hull CW
    arr.sort(key=lambda x: (-x[1], x[0]))
    if len(arr) <= 2:
        return arr
    lower = []
    for dl in arr:
        while len(lower) > 1 and cross(lower[-2], lower[-1], lower[-1], dl) >= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(arr):
        while len(upper) > 1 and cross(upper[-2], upper[-1], upper[-1], du) >= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


for _ in range(int(sys.stdin.readline().strip())):
    hull_cnd = []
    n = int(sys.stdin.readline().strip())
    for _ in range(n//5):
        x1, y1, x2, y2, x3, y3, x4, y4, x5, y5 = map(int, sys.stdin.readline().strip().split())
        hull_cnd += [(x1, y1), (x2, y2), (x3, y3), (x4, y4), (x5, y5)]
    if n % 5:
        h = tuple(map(int, sys.stdin.readline().strip().split()))
        for i in range(0, n % 5 * 2, 2):
            hull_cnd.append((h[i], h[i+1]))
    hull = monotone_chain_r(hull_cnd)
    print(len(hull))
    for row in hull:
        print(*row)
