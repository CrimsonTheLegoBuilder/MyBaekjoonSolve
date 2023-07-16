import sys
from math import inf

ax, ay, bx, by, cx, cy, dx, dy = map(int, sys.stdin.readline().strip().split())
v1 = [bx - ax, by - ay]
v2 = [dx - cx, dy - cy]


def distance(x1, vx1, x2, vx2, y1, vy1, y2, vy2):
    return (((x1 + vx1) - (x2 + vx2)) ** 2 + ((y1 + vy1) - (y2 + vy2)) ** 2) ** 0.5


den = int(1e10)  # denominator`s value
start, end = 1, den

while end - start >= 3:
    mid_1 = (start * 2 + end) // 3  # 1/3
    mid_2 = (start + end * 2) // 3  # 2/3
    pos_1 = mid_1 / den  # at 1/3 pos.
    pos_2 = mid_2 / den  # at 2/3 pos.
    if distance(ax, v1[0] * pos_1, cx, v2[0] * pos_1, ay, v1[1] * pos_1, cy, v2[1] * pos_1) <\
            distance(ax, v1[0] * pos_2, cx, v2[0] * pos_2, ay, v1[1] * pos_2, cy, v2[1] * pos_2):
        end = mid_2
    else:
        start = mid_1

result = inf
for i in range(start, end + 1):
    ratio = i / den
    ans = distance(ax, v1[0] * ratio, cx, v2[0] * ratio, ay, v1[1] * ratio, cy, v2[1] * ratio)
    result = min(result, ans)

# print(start, end)
# print(distance(ax, v1[0] * pos_1, cx, v2[0] * pos_1, ay, v1[1] * pos_1, cy, v2[1] * pos_1),
#       distance(ax, v1[0] * pos_2, cx, v2[0] * pos_2, ay, v1[1] * pos_2, cy, v2[1] * pos_2))
print(result)
