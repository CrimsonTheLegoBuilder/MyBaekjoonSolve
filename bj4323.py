import sys
from math import pi, acos, isclose


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


def cal_dist_square(d1, d2):  # get c^2 = a^2 + b^2
    return (d1[0] - d2[0])**2 + (d1[1] - d2[1])**2


def get_theta(d1, d2, d3, d4):
    force = dot(d1, d2, d3, d4)
    r1 = cal_dist(d1, d2)
    r2 = cal_dist(d3, d4)
    cos_t = (force/r1) / r2
    if isclose(abs(cos_t), 1.0):
        cos_t = 1.0 * (-1*cos_t < 0)
    # print(cos_t)
    return pi - acos(cos_t)


while True:
    N = int(sys.stdin.readline().strip())
    if not N:
        break
    tmpA = list(map(int, sys.stdin.readline().strip().split()))
    A = [(tmpA[2*i], tmpA[2*i+1]) for i in range(N)]
    tmpB = list(map(int, sys.stdin.readline().strip().split()))
    B = [(tmpB[2*i], tmpB[2*i+1]) for i in range(N)]
    flag = 0
    vA = []
    vB = []
    for i in range(N):
        r_1 = cal_dist_square(A[i-1], A[i])
        r_2 = cal_dist_square(B[i-1], B[i])
        theta1 = get_theta(A[i-1], A[i], A[i], A[(i+1) % N])
        theta2 = get_theta(B[i-1], B[i], B[i], B[(i+1) % N])
        CCW1 = 1 if cross(A[i-1], A[i], A[i], A[(i+1) % N]) >= 0 else -1
        CCW2 = 1 if cross(B[i-1], B[i], B[i], B[(i+1) % N]) >= 0 else -1
        if not isclose(theta1, theta2) or CCW1 != CCW2:
            flag = 1
        vA.append(r_1)
        vB.append(r_2)
    ratio = vB[0]//vA[0]
    for i in range(1, N):
        if ratio != vB[i]//vA[i]:
            flag = 1
    print('dissimilar' if flag else 'similar')
