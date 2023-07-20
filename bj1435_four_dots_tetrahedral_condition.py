import sys


def judge_triangle(arr):
    arr1 = sorted(arr)
    if arr1[2] > arr1[1] + arr1[0]:
        return 0  # triangle condition not satisfied
    else:
        return 1  # triangle condition satisfied


def cal_projection(a, b, c):  # calculate length of projection
    x = (a**2 - b**2 + c**2) / (2*c)
    return x


def cal_height(a, x):  # calculate height
    return (a**2 - abs(x)**2)**0.5


def judge_tetrahedron(ha1, ha2, xa1, xa2, d):  # tetrahedral condition for th last side d
    if ((ha1 - ha2)**2 + (xa1 - xa2)**2 > d**2) | ((ha1 + ha2)**2 + (xa1 - xa2)**2 < d**2):
        print('NO')
    else:
        print('YES')


side = [list(map(int, sys.stdin.readline().strip().split())) for i in range(4)]
if (judge_triangle([side[0][1], side[0][2], side[1][2]]) == 0) | (judge_triangle([side[0][1], side[0][3], side[1][3]]) == 0) |\
        (judge_triangle([side[0][2], side[0][3], side[2][3]]) == 0) | (judge_triangle([side[1][2], side[1][3], side[2][3]]) == 0):
    print('NO')
else:
    x1, x2 = cal_projection(side[0][1], side[0][2], side[1][2]), cal_projection(side[1][3], side[2][3], side[1][2])
    h1, h2 = cal_height(side[0][1], x1), cal_height(side[1][3], x2)
    judge_tetrahedron(h1, h2, x1, x2, side[0][3])  # final check
