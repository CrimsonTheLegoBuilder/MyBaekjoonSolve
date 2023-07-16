import sys


def cross_abs(v1, v2):
    ans = ((v1[1] * v2[2] - v1[2] * v2[1]) ** 2 + (v1[2] * v2[0] - v1[0] * v2[2]) ** 2 + (v1[0] * v2[1] - v1[1] * v2[0]) ** 2) ** 0.5
    return ans


def dot(v1, v2):
    ans = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]
    return ans


ax, ay, az, bx, by, bz, cx, cy, cz = map(int, sys.stdin.readline().strip().split())
v_s1 = [bx - ax, by - ay, bz - az]
v_s2 = [ax - bx, ay - by, az - bz]
v_d1 = [cx - ax, cy - ay, cz - az]
v_d2 = [cx - bx, cy - by, cz - bz]
l_s = (v_s1[0] ** 2 + v_s1[1] ** 2 + v_s1[2] ** 2) ** 0.5
l_1 = (v_d1[0] ** 2 + v_d1[1] ** 2 + v_d1[2] ** 2) ** 0.5
l_2 = (v_d2[0] ** 2 + v_d2[1] ** 2 + v_d2[2] ** 2) ** 0.5

if (dot(v_s1, v_d1) > 0) & (dot(v_s2, v_d2) > 0):
    print(cross_abs(v_s1, v_d1) / l_s)
else:
    print(min(l_1, l_2))
