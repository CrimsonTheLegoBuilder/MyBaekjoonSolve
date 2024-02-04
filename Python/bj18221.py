import sys

N = int(sys.stdin.readline().strip())
grid = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
r1, r2, c1, c2 = 0, 0, 0, 0
for i in range(N):
    for j in range(N):
        if grid[i][j] == 2:
            r1, c1 = i, j
        if grid[i][j] == 5:
            r2, c2 = i, j
r1, r2 = min(r1, r2), max(r1, r2)
c1, c2 = min(c1, c2), max(c1, c2)
cnt = 0
for i in range(r1, r2+1):
    for j in range(c1, c2+1):
        cnt += grid[i][j] == 1
print(int(cnt >= 3))
