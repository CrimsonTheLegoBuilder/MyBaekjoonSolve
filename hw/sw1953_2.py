from collections import deque


def bfs(r, c):
    visited[r][c] = 1
    s = 1
    queue.append((r, c))
    while queue:
        i, j = queue.popleft()
        if visited[i][j] == L:
            return s
        for k in range(4):
            ni = i + drc[k][0]
            nj = j + drc[k][1]
            cur = grid[i][j]
            if 0 <= ni < N and 0 <= nj < M and grid[ni][nj] and not visited[ni][nj] \
                    and able[cur][k] and able[grid[ni][nj]][3-k]:
                queue.append((ni, nj))
                visited[ni][nj] = visited[i][j] + 1
                s += 1
    return s


for tc in range(int(input())):
    N, M, R, C, L = map(int, input().split())
    grid = [tuple(map(int, input().split())) for _ in range(N)]
    visited = [[0]*M for _ in range(N)]
    queue = deque()
    drc = ((1, 0), (0, 1), (0, -1), (-1, 0))
    able = ((0, 0, 0, 0), (1, 1, 1, 1), (1, 0, 0, 1), (0, 1, 1, 0),
            (0, 1, 0, 1), (1, 1, 0, 0), (1, 0, 1, 0), (0, 0, 1, 1))
    ans = bfs(R, C)
    print(f'#{tc+1} {ans}')

    # for row in visited:
    #     print(*row)

'''
1
5 6 2 1 3
0 0 5 3 6 0
0 0 2 0 2 0
3 3 1 3 7 0
0 0 0 0 0 0
0 0 0 0 0 0
'''
