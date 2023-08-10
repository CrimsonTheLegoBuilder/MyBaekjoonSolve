import sys
from collections import deque


def cal_dist(d1, d2):  # get c^2 = a^2 + b^2
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


def bfs(v):
    queue.append(v)
    visited[v] += 1
    while queue:
        p = queue.popleft()
        # print(p)
        for w in graph[p]:
            if not ~visited[int(w)]:
                visited[int(w)] = visited[p] + 1
                queue.append(w)


V, m = map(float, sys.stdin.readline().strip().split())
xs, ys = map(float, sys.stdin.readline().strip().split())
xt, yt = map(float, sys.stdin.readline().strip().split())
limit = m * 60.0
vertices = [(xs, ys)]
while True:
    try:
        x, y = map(float, sys.stdin.readline().strip().split())
        vertices.append((x, y))
    except ValueError:
        break
vertices.append((xt, yt))
# print(vertices)

n = len(vertices)
graph = [[] for _ in range(n)]
# print(graph)
visited = [-1] * n
queue = deque()
for i in range(n):
    for j in range(n):
        if i == j:
            continue
        if cal_dist(vertices[i], vertices[j]) < V * limit:
            graph[i].append(j)
# print(graph)
bfs(0)
# print(visited)
ans = visited[n-1]
if ~ans:
    print(f'Yes, visiting {ans-1} other holes.')
else:
    print('No.')

'''
3 1
0.000 0.000
700.000 0.000
179.000 0.000
358.000 0.000

3 1
0.000 0.000
1000.000 200.000
179.000 0.000
200.000 100.000
300.000 50.000
358.000 0.000
400.000 50.000
500.000 10.000
600.000 10.000
751.000 20.000
700.000 10.000
680.000 0.000
500.000 30.000
400.000 30.000
300.000 25.000
800.000 20.000
900.000 15.000
1000.000 40.000
900.000 20.000
850.000 15.000
'''