import sys


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


def dfs(v, p):
    visited[v] = visited[p] + 1 if ~p else 1
    for w in graph[v]:
        if w == p:
            continue
        if not visited[w]:
            dfs(w, v)


for tc in range(1, int(sys.stdin.readline().strip()) + 1):
    H, W, N = map(int, sys.stdin.readline().strip().split())
    craters = [list(map(float, sys.stdin.readline().strip().split())) for _ in range(N)]
    visited = [0] * (N+2)
    graph = [[] for _ in range(N+2)]
    for i in range(N):
        x1, y1, r1 = craters[i]
        if y1 + r1 > H:
            graph[0].append(i+1)
            graph[i+1].append(0)
        if y1 - r1 < 0:
            graph[N+1].append(i+1)
            graph[i+1].append(N+1)
        for j in range(i, N):
            if i == j:
                continue
            _, _, r2 = craters[j]
            if cal_dist(craters[i], craters[j]) < r1 + r2:
                graph[j+1].append(i+1)
                graph[i+1].append(j+1)
    # print(graph)
    # print(visited)
    dfs(0, -1)
    # print(visited)
    if visited[-1]:
        print(f'Case {tc}: Find Another Path')
    else:
        print(f'Case {tc}: Clear To Go')

'''
2
100 200 3
50.000 90.000 20.000
70.000 60.000 20.000
100.000 20.000 25.000
10 20 2
3.000 2.000 7.000
4.000 5.000 6.500
'''
