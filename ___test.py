# import sys
# import math
#
#
# def cross(d1, d2, d3):  # cross product / CCW / get area
#     return (d2[1] - d1[1]) * (d3[2] - d2[2]) - (d2[2] - d1[2]) * (d3[1] - d2[1])
#
#
# def in_triangle(arr):
#     flag = 0
#     arr.sort(key=lambda d: d[2])  # sort on y
#     miny, maxy = arr[0], arr[-1]
#     hull_candidate = [miny, maxy]
#     length_of_v = ((miny[1] - maxy[1])**2 + (miny[2] - maxy[2])**2) ** 0.5
#     distance_min = 1e12
#     distance_max = -1e12
#     max_r, max_l = [], []
#     for i in arr[1:-1]:
#         length = cross(maxy, miny, i) / length_of_v
#         if cross(maxy, miny, i) == 0:
#             flag += 1
#         elif length > 0 and distance_max < length:
#             if len(max_r):
#                 max_r.pop()
#             max_r.append(i)
#             distance_max = length
#         elif length < 0 and distance_min > length:
#             if len(max_l):
#                 max_l.pop()
#             max_l.append(i)
#             distance_min = length
#     if flag == len(arr) - 2:
#         return [miny, maxy]  # if all stars are standing in a straight line
#     elif len(max_l) == 0:
#         for j in arr[1:-1]:
#             if j == max_r[0]:
#                 hull_candidate.append(j)
#             if cross(miny, max_r[0], j) < 0 or cross(maxy, max_r[0], j) > 0:
#                 hull_candidate.append(j)
#     elif len(max_r) == 0:
#         for j in arr[1:-1]:
#             if j == max_l[0]:
#                 hull_candidate.append(j)
#             if cross(miny, max_l[0], j) > 0 or cross(maxy, max_l[0], j) < 0:
#                 hull_candidate.append(j)
#     else:
#         for j in arr[1:-2]:
#             if j == max_l[0] or j == max_r[0]:
#                 continue
#             if cross(miny, max_l[0], j) > 0 or cross(maxy, max_l[0], j) < 0 or cross(miny, max_r[0], j) < 0 or cross(maxy, max_r[0], j) > 0:
#                 hull_candidate.append(j)
#     return hull_candidate
#
#
# n, t = map(int, sys.stdin.readline().strip().split())
# stars = []
# for _ in range(n):
#     dx, dy, vx, vy = map(int, sys.stdin.readline().strip().split())
#     stars.append([0.0, dx, dy, vx, vy])
#
# print(in_triangle(stars))

print(tuple('Hello'))