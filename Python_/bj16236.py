import sys
from heapq import heappop as h_pop
from heapq import heappush as h_push


def bfs(i, j):
    global V, cnt
    h = []
    h_push(h, (0, i, j))
    ocean[i][j] = 0
    visited[i][j] = 0
    while h:
        _, r, c = h_pop(h)
        if 0 < ocean[r][c] < V:
            ocean[r][c] = 9
            cnt += 1
            if cnt == V:
                V += 1
                cnt = 0
            return r, c, visited[r][c]
        for k in range(4):
            nr, nc = r + drc[k][0], c + drc[k][1]
            if 0 <= nr < N and 0 <= nc < N and not ~visited[nr][nc] and not ocean[nr][nc] > V:
                if not ocean[nr][nc]:
                    visited[nr][nc] = visited[r][c] + 1
                    h_push(h, (visited[nr][nc], nr, nc))
                elif ocean[nr][nc] <= V:
                    visited[nr][nc] = visited[r][c] + 1
                    h_push(h, (visited[nr][nc], nr, nc))
    return 0, 0, 0


def find_start():
    for i in range(N):
        for j in range(N):
            if ocean[i][j] == 9:
                return i, j


N = int(sys.stdin.readline().strip())
ocean = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
drc = ((-1, 0), (0, -1), (0, 1), (1, 0))
V, cnt = 2, 0
total = 0
sr, sc = find_start()
# print(sr, sc)
while 1:
    visited = [[-1]*N for _ in range(N)]
    sr, sc, d = bfs(sr, sc)
    # for row in visited:
    #     print(*row)
    # print()
    # for row in ocean:
    #     print(*row)
    # print()
    if not d:
        break
    total += d
# print(total)

'''
20
0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 2 3 3 4 4
0 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
9 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6
0 0 0 0 0 0 0 0 0 0 0 0 0 1 2 2 3 3 4 4
6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 4

5
0 0 0 0 0
1 0 0 0 2
0 0 3 3 0
3 3 0 0 9
1 3 0 0 0
'''