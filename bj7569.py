import sys
from collections import deque


def bfs():
    dh = (0, 0, 0, 0, 1, -1)
    dn = (0, 0, 1, -1, 0, 0)
    dm = (1, -1, 0, 0, 0, 0)
    while queue:
        h, n, m = queue.popleft()
        for i in range(6):
            nh = h + dh[i]
            nn = n + dn[i]
            nm = m + dm[i]
            if 0 <= nh < H and 0 <= nn < N and 0 <= nm < M and not box[nh][nn][nm]:
                box[nh][nn][nm] = box[h][n][m] + 1
                queue.append((nh, nn, nm))


M, N, H = map(int, sys.stdin.readline().strip().split())
box = [[list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)] for _ in range(H)]
queue = deque()

for z in range(H):
    for y in range(N):
        for x in range(M):
            if box[z][y][x] == 1:
                queue.append((z, y, x))

bfs()

latest = 1
for z in range(H):
    for y in range(N):
        for x in range(M):
            if box[z][y][x] == 0:
                print(-1)
                exit(0)
            if latest < box[z][y][x]:
                latest = box[z][y][x]
print(latest - 1)
