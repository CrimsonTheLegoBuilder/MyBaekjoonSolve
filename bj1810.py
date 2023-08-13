import sys
from collections import deque


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


N, F = map(int, sys.stdin.readline().strip().split())
limit = 1_000_001
dots = [(0, 0)]
graph = [[] for _ in range(N)]
dist = [0] + [limit*5] * N
queue = deque()
linked_dic = [dict() for _ in range(limit)]
linked_dic[0][0] = 0
for i in range(1, N+1):
    x, y = map(int, sys.stdin.readline().strip().split())
    dots.append((x, y))
    linked_dic[x][y] = i
    # print(linked_dic[x])
for d in range(N):
    i, j = dots[d]
    for x in range(-2, 3, 1):
        for y in range(-2, 3, 1):
            if not x and not y:
                continue
            ni = i + x
            nj = j + y
            if 0 <= ni < limit and 0 <= nj <= F:
                if linked_dic[ni].get(nj):
                    graph[d].append(((linked_dic[ni][nj]), cal_dist((i, j), (ni, nj))))
print(graph)
print(dist)


'''
5 3
1 2
6 3
4 1
3 2
0 2

'''