import sys
from collections import deque


def bfs(x, y):
    queue.append((x, y))
    visited[x][y] = 1
    flag = 1
    start = grid[x][y]
    while queue:
        r, c = queue.popleft()
        cur = grid[r][c]
        for z in range(8):
            nr = r + dr[z]
            nc = c + dc[z]
            if 0 <= nr < N and 0 <= nc < M and grid[nr][nc] <= cur and not visited[nr][nc]:
                queue.append((nr, nc))
                visited[nr][nc] = 1
            if 0 <= nr < N and 0 <= nc < M and grid[nr][nc] > start:
                flag = 0
        # print(queue)
    if flag:
        # top_candidate.add((x, y))
        top_candidate.append((x, y))


# def dfs(r, c):
#     visited[r][c] = 0
#     cur = grid[r][c]
#     for x in range(8):
#         nr = r + dr[x]
#         nc = c + dc[x]
#         if 0 <= nr < N and 0 <= nc < M and grid[nr][nc] == cur and visited[nr][nc]:
#             for y in range(8):
#                 flag = 1
#                 tr = nr + dr[y]
#                 tc = nc + dc[y]
#                 if 0 <= tr < N and 0 <= tc < M and grid[tr][tc] < grid[nr][nc]:
#                     flag = 0
#                 if flag:
#                     dfs(nr, nc)


N, M = map(int, sys.stdin.readline().strip().split())
grid = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
visited = [[0] * M for _ in range(N)]
queue = deque()
# top_candidate = set()
top_candidate = []
dr = (1, 1, -1, -1, 0, 1, 0, -1)
dc = (1, -1, -1, 1, 1, 0, -1, 0)
for i in range(N):
    for j in range(M):
        if not visited[i][j]:
            bfs(i, j)
            # for u in range(N):
            #     print(*visited[u])


# cnt = 0
# for i, j in top_candidate:
#     if visited[i][j]:
#         dfs(i, j)
#         cnt += 1
# for u in range(N):
#     print(*visited[u])

# print(cnt)
# print(top_candidate)
print(len(top_candidate))

'''
8 10
4 3 2 2 1 2 5 5 6 6
3 3 3 2 1 2 4 5 5 6
2 2 2 2 1 2 3 3 3 3
2 1 1 1 1 2 2 3 4 5
1 1 0 0 0 1 2 2 2 3
0 0 0 1 1 1 0 1 1 1
0 1 2 1 1 1 0 1 0 1
0 1 1 1 2 1 0 0 0 0

8 10
1 2 3 2 1 2 5 5 6 6
2 2 3 2 1 2 4 5 5 6
3 3 3 2 1 2 3 3 3 3
3 2 1 1 1 2 2 3 4 5
2 1 0 0 0 1 2 2 2 3
1 0 0 1 1 1 0 1 1 1
0 0 2 1 1 1 0 1 0 1
0 1 1 1 2 1 0 0 0 0
'''
