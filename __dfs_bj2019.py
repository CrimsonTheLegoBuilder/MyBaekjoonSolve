import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def intersect(arr):
    flag = 1
    l = len(arr)
    if l <= 2:
        flag = 0
    for d in range(l):
        for dx in range(d+1, d+l-1):
            if cross(arr[d-1], arr[d], arr[d], arr[dx%l]) == 0 and dot(arr[d-1], arr[dx%l], arr[dx%l], arr[d]) > 0:
                flag = 0
    #         print(flag)
    # print()
    for d in range(l):
        for dx in range(d+1, d+l-2):
            if cross(arr[d], arr[d-1], arr[d-1], arr[dx%l]) * cross(arr[d-1], arr[d], arr[d], arr[(dx+1)%l]) > 0 and \
                    cross(arr[(dx+1)%l], arr[dx%l], arr[dx%l], arr[d-1]) * cross(arr[dx%l], arr[(dx+1)%l], arr[(dx+1)%l], arr[d]) > 0:
                flag = 0
            # print(flag)
    return flag


def dfs(v, p):
    global closed
    visited[v] = visited[p] + 1 if ~p else 1
    # temp.append(v)
    for w in linked_list[v]:
        if w == p:
            continue
        if not visited[w]:
            if len(linked_list[w]) != 2:
                closed = 0
            xa, ya, xb, yb = nodes[v]
            xc, yc, xd, yd = nodes[w]
            if xa == xc and ya == yc or xa == xd and ya == yd:
                hull.append((xa, ya))
            if xb == xc and yb == yc or xb == xd and yb == yd:
                hull.append((xb, yb))
            dfs(w, v)
        elif visited[w] < visited[v]:
            xa, ya, xb, yb = nodes[v]
            xc, yc, xd, yd = nodes[w]
            if xa == xc and ya == yc or xa == xd and ya == yd:
                hull.append((xa, ya))
            if xb == xc and yb == yc or xb == xd and yb == yd:
                hull.append((xb, yb))


N = int(sys.stdin.readline().strip())
nodes = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
linked_list = []
visited = [0] * N
for i in range(N):
    temp = []
    for j in range(N):
        if i == j:
            continue
        x1, y1, x2, y2 = nodes[i]
        x3, y3, x4, y4 = nodes[j]
        if x1 == x3 and y1 == y3 or x1 == x4 and y1 == y4 or x2 == x3 and y2 == y3 or x2 == x4 and y2 == y4:
            temp.append(j)
    linked_list.append(temp)
# print(linked_list)

graph = []
for i in range(N):
    hull = []
    closed = 1
    if not visited[i]:
        dfs(i, -1)
        if closed:
            graph.append(hull)
# print(visited)
# print(graph)

cnt = 0
for dots in graph:
    cnt += intersect(dots)
print(cnt)

'''
5
0 0 2 0
2 0 2 2
2 2 0 2
0 2 1 1
1 1 0 0

5
0 0 2 0
2 0 2 2
2 2 0 2
0 2 2 1
2 1 0 0

4
0 0 2 0
2 0 0 2
0 2 2 2
2 2 0 0
'''