import sys
# Thank you dlwoals0850!!!


def cal_texi_dist(d1, d2):
    return abs(d1[0]-d2[0]) + abs(d1[1]-d2[1])


N, T = map(int, sys.stdin.readline().strip().split())
graph = [[0]*N for _ in range(N)]
info = []
special = set()
for i in range(N):
    si, xi, yi = map(int, sys.stdin.readline().strip().split())
    info.append((xi, yi))
    if si:
        special.add(i)

for i in range(N):
    ci = info[i]
    for j in range(i+1, N):
        cj = info[j]
        dist = cal_texi_dist(ci, cj)
        graph[i][j] = dist
        graph[j][i] = dist

to_spec = [0]*N
for not_spec in range(N):
    if not_spec not in special:
        ci = info[not_spec]
        min_dist = 2001
        for spec in special:
            cj = info[spec]
            min_dist = min(cal_texi_dist(ci, cj), min_dist)
        to_spec[not_spec] = min_dist

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
