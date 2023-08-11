import sys
from collections import deque


def cross_s(s1, s2):  # s_ = (No, x1, y1, x2, y2)
    result1 = (s1[3] - s1[1]) * (s2[2] - s1[4]) - (s1[4] - s1[2]) * (s2[1] - s1[3])
    result2 = (s1[1] - s1[3]) * (s2[4] - s1[2]) - (s1[2] - s1[4]) * (s2[3] - s1[1])
    result3 = (s2[3] - s2[1]) * (s1[2] - s2[4]) - (s2[4] - s2[2]) * (s1[1] - s2[3])
    result4 = (s2[1] - s2[3]) * (s1[4] - s2[2]) - (s2[2] - s2[4]) * (s1[3] - s2[1])
    flag1 = (result1 * result2 > 0) * (result3 * result4 > 0)
    if result1 == 0 and (s2[1] - s1[1]) * (s1[3] - s2[1]) + (s2[2] - s1[2]) * (s1[4] - s2[2]) >= 0:
        flag1 = 1
    if result2 == 0 and (s2[3] - s1[1]) * (s1[3] - s2[3]) + (s2[4] - s1[2]) * (s1[4] - s2[4]) >= 0:
        flag1 = 1
    if result3 == 0 and (s1[1] - s2[1]) * (s2[3] - s1[1]) + (s1[2] - s2[2]) * (s2[4] - s1[2]) >= 0:
        flag1 = 1
    if result4 == 0 and (s1[3] - s2[1]) * (s2[3] - s1[3]) + (s1[4] - s2[2]) * (s2[4] - s1[4]) >= 0:
        flag1 = 1
    return flag1


def cross_d(d, s):  # d = (x, y), s = (No, x1, y1, x2, y2)
    result1 = (s[3] - s[1]) * (d[1] - s[4]) - (s[4] - s[2]) * (d[0] - s[3])
    result2 = (d[0] - s[1]) * (s[3] - d[0]) + (d[1] - s[2]) * (s[4] - d[1])
    return (result1 == 0) * (result2 >= 0)


def bfs():
    visited[0] += 1
    while queue:
        p = queue.popleft()
        for w in graph[p]:
            if not ~visited[w]:
                visited[w] = visited[p] + 1
                queue.append(w)


M, N = map(int, sys.stdin.readline().strip().split())
k = int(sys.stdin.readline().strip())
bus = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(k)]
xs, ys, xg, yg = map(int, sys.stdin.readline().strip().split())
graph = [[] for _ in range(k+2)]
visited = [-1] * (k+2)
queue = deque([0])
for i in range(k):
    if cross_d((xs, ys), bus[i]):
        graph[0].append(bus[i][0])
        # graph[bus[i][0]].append(0)
# print(graph)
for i in range(k):
    for j in range(i+1, k):
        if cross_s(bus[i], bus[j]):
            graph[bus[i][0]].append(bus[j][0])
            graph[bus[j][0]].append(bus[i][0])
# print(graph)
for i in range(k):
    if cross_d((xg, yg), bus[i]):
        # graph[k+1].append(bus[i][0])
        graph[bus[i][0]].append(k+1)
# print(graph)
bfs()
# print(visited)
print(visited[k+1]-1)
