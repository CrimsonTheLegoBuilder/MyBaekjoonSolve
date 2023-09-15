import sys
from math import acos, sin, cos, isclose, atan, atan2, degrees
#
# print(atan2(10, 0))
# print(atan(10/0))

# print(acos(4))
# def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
#     return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])
# a = cross((96, -95), (-97, 100), (-97, 100), (-95, 98))
# print(a)

# print(73428**.5)

# a = (524+196)**2 + 44**2
# b = (524+44)**2 + 196**2
# c = (524+5)**2 + (196+49)**2
# print(a, b, c)
#
# '''
# 383 49 524 196 44 524
# 0 0 0 0 0 0
# '''
#
#
# n = int(input())
# time = []
# for _ in range(n) :
#     time.append(list(map(int, input().split())))  # t, s
# time.sort(key=lambda x:x[1], reverse=True)
#
# no_answer = False
# ans = time[0][1] - time[0][0]
# for i in range(1, n) :
#     if time[i][0] > ans or ans < 0 :  # 불가능
#         no_answer = True
#         break
#     if time[i][1] < ans :
#         ans = time[i][1] - time[i][0]
#     else :
#         ans -= time[i][0]
#
# if no_answer :
#     print(-1)
# else :
#     print(ans)
#
#
# '''
# 3
# 4 21
# 13 19
# 4 20
# '''

# T = int(input())
# for test_case in range(1, T+1):
#     M, N, K = map(int, input().split())
#     adj = [[0] * M for _ in range(N)]
#     visited = [[0] * M for _ in range(N)]
#     for _ in range(K):
#         X, Y = map(int, input().split())
#         adj[Y][X] = 1
#     for row in adj:
#         print(*row)
#     print()

# print(degrees(atan2(-2, 10)))


def facto(n):
    return n*facto(n-1) if n else 1


print(facto(10))

