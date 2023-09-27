import sys
from collections import deque
sys.setrecursionlimit(10000)


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def dot(d1, d2, d3, d4):  # dot product / get projection / get Force
    return (d2[0] - d1[0]) * (d4[0] - d3[0]) + (d2[1] - d1[1]) * (d4[1] - d3[1])


def bfs(vr, vc, depth):
    dr = (1, 1, 1, 0, 0, 0, -1, -1, -1)
    dc = (1, 0, -1, 1, 0, -1, 1, 0, -1)
    while queue:
        r, c = queue.popleft()
        for x in range(9):
            nr = r + vr + dr[x]
            nc = c + vc + dc[x]
            flag = 0
            for f in range(4):
                if cross((r, c), (nr, nc), (nr, nc), final[f-1]) * \
                        cross((nr, nc), (r, c), (r, c), final[f]) > 0 and \
                   cross(final[f-1], final[f], final[f], (r, c)) * \
                        cross(final[f], final[f-1], final[f-1], (nr, nc)) > 0:
                    flag = 1


N, M = map(int, sys.stdin.readline().strip().split())
arena = [str(sys.stdin.readline().strip()) for _ in range(N)]
queue = deque()
rf, cf = None, None
for i in range(N):
    for j in range(M):
        if arena[i][j] == 'S':
            queue.append((i, j))
        if arena[i][j] == 'F':
            rf, cf = i, j
vi, vj = map(int, sys.stdin.readline().strip().split())
final = ((rf + 0.5, cf + 0.5), (rf - 0.5, cf + 0.5), (rf - 0.5, cf - 0.5), (rf + 0.5, cf - 0.5))

bfs(vi, vj, 0)

