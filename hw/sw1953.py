from collections import deque


def bfs(r, c):
    visited[r][c] = 1
    queue.append((r, c))
    while queue:
        i, j = queue.popleft()
        for ij in drc[grid[i][j]]:
            ni = i + ij[0]
            nj = j + ij[1]
            if 0 <= ni < N and 0 <= nj < M and grid[ni][nj] and not visited[ni][nj]\
                    and visited[i][j] < L:
                if ij[0] == 1 and ij[1] == 0 and grid[ni][nj] in (1, 2, 4, 7) or \
                        ij[0] == -1 and ij[1] == 0 and grid[ni][nj] in (1, 2, 5, 6) or \
                        ij[0] == 0 and ij[1] == 1 and grid[ni][nj] in (1, 3, 6, 7) or \
                        ij[0] == 0 and ij[1] == -1 and grid[ni][nj] in (1, 3, 4, 5):
                    queue.append((ni, nj))
                    visited[ni][nj] = visited[i][j] + 1


for tc in range(int(input())):
    N, M, R, C, L = map(int, input().split())
    grid = [tuple(map(int, input().split())) for _ in range(N)]
    visited = [[0]*M for _ in range(N)]
    queue = deque()
    drc = ((), ((1, 0), (0, 1), (-1, 0), (0, -1)), ((1, 0), (-1, 0)), ((0, 1), (0, -1)),
           ((-1, 0), (0, 1)), ((0, 1), (1, 0)), ((1, 0), (0, -1)), ((0, -1), (-1, 0)))
    bfs(R, C)
    ans = 0
    for x in range(N):
        for y in range(M):
            if visited[x][y]:
                ans += 1
    # for row in visited:
    #     print(*row)
    print(f'#{tc+1} {ans}')


'''
1
5 6 2 1 3
0 0 5 3 6 0
0 0 2 0 2 0
3 3 1 3 7 0
0 0 0 0 0 0
0 0 0 0 0 0
'''
