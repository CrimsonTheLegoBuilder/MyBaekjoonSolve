import sys


def dfs(d, r, c):
    cnt = 0
    i, j = r, c
    i_, j_ = drc[d][0], drc[d][1]
    while 1:
        i += i_
        j += j_
        cnt += 1
        # print(d, cnt)
        if i < 0 or i >= N or j < 0 or j >= M:
            return 0, cnt
        if grid[i][j] == 'C':
            return 0, cnt
        if i == r and j == c:
            return 1, cnt
        if grid[i][j] == '/':
            i_, j_ = -j_, -i_
        if grid[i][j] == '\\':
            i_, j_ = j_, i_


drc = ((-1, 0), (0, 1), (1, 0), (0, -1))
D = ['U', 'R', 'D', 'L']
INF = int(12e12)
N, M = map(int, sys.stdin.readline().strip().split())
grid = [sys.stdin.readline().strip() for _ in range(N)]
ir, ic = map(int, sys.stdin.readline().strip().split())
ans = ['S', 0]
for x in range(4):
    a, t = dfs(x, ir-1, ic-1)
    if a and ans[1] < t:
        ans[0] = D[x]
        ans[1] = INF
        # print(*ans)
    elif ans[1] < t:
        ans[0] = D[x]
        ans[1] = t
print(f'{ans[0]}\n{ans[1] if ans[1] < INF else "Voyager"}')
