import sys
# Thank you dlwoals0850!!!


def cal_texi_dist(x1, y1, x2, y2):
    return abs(x1-x2) + abs(y1-y2)


N, T = map(int, sys.stdin.readline().strip().split())
graph = [[0]*N for _ in range(N)]
info = []
special = set()
not_special = set()

for i in range(N):
    si, xi, yi = map(int, sys.stdin.readline().strip().split())
    info.append((xi, yi))
    if si:
        special.add(i)
    else:
        not_special.add(i)

for i in range(N):
    xi, yi = info[i]
    for j in range(i+1, N):
        xj, yj = info[j]
        dist = cal_texi_dist(xi, yi, xj, yj)
        graph[i][j] = dist
        graph[j][i] = dist

to_spec = [0]*N
for i in not_special:
    xi, yi = info[i]
    min_dist = 2001
    for j in special:
        xj, yj = info[j]
        # if cal_texi_dist(xi, yi, xj, yj) < min_dist:
        #     min_dist = cal_texi_dist(xi, yi, xj, yj)
        min_dist = min(cal_texi_dist(xi, yi, xj, yj), min_dist)
    to_spec[i] = min_dist

for i in range(N):
    for j in range(i+1, N):
        graph[i][j] = min(graph[i][j], to_spec[i] + T + to_spec[j])
        graph[j][i] = graph[i][j]

# for row in graph:
#     print(*row)
M = int(sys.stdin.readline().strip())
for _ in range(M):
    s, e = map(int, sys.stdin.readline().strip().split())
    print(graph[s-1][e-1])
