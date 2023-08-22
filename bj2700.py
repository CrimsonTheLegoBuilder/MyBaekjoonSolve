import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


for _ in range(int(sys.stdin.readline().strip())):
    N = int(sys.stdin.readline().strip())
    hull = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
    inside = [[i-30, 30, -30] for i in range(61)]
    for i in range(N):
        d_1 = hull[i]
        x1, y1 = d_1
        d_2 = hull[(i+1) % N]
        x2, y2 = d_2
        if y1 == y2:
            continue
        if y1 < y2:
            for y in range(y1+1, y2+1):
                for x in range(-30, 31):
                    if cross(d_2, d_1, d_1, (x, y)) > 0:
                        inside[y+30][1] = x
                        break
        if y1 > y2:
            for y in range(y2, y1):
                for x in range(31, -31, -1):
                    if cross(d_2, d_1, d_1, (x, y)) > 0:
                        inside[y+30][2] = x
                        break
    ans = []
    for row in reversed(inside):
        # print(*row)
        _, x1, x2 = row
        if x1 <= x2:
            ans.append(row)
    print(len(ans))
    for row in ans:
        print(*row)
