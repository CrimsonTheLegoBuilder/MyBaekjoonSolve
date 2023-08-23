import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


for _ in range(int(sys.stdin.readline().strip())):
    N = int(sys.stdin.readline().strip())
    inner = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
    M = int(sys.stdin.readline().strip())
    outer = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(M)]
    ans = 12e12
    for i in range(M):
        for d in inner:
            torque = cross(outer[i-1], outer[i], outer[i], d)
            arm = cal_dist(outer[i-1], outer[i])
            if dot(outer[i-1], d, d, outer[i]) < 0:
                dist = min(cal_dist(outer[i-1], d), cal_dist(outer[i], d))
                ans = min(ans, dist)
            else:
                dist = abs(torque) / arm
                ans = min(ans, dist)
    for j in range(N):
        for d in outer:
            torque = cross(inner[j-1], inner[j], inner[j], d)
            arm = cal_dist(inner[j-1], inner[j])
            if dot(inner[j-1], d, d, inner[j]) < 0:
                dist = min(cal_dist(inner[j-1], d), cal_dist(inner[j], d))
                ans = min(ans, dist)
            else:
                dist = abs(torque) / arm
                ans = min(ans, dist)
    print(ans/2)
