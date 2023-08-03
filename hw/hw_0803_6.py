# def dfs(r, c):
#     global ans
#     visited[r][c] = 1
#     if r == 0:
#         ans = c
#         return
#     dc = [-1, 1]
#     for i in range(2):
#         nc = c + dc[i]
#         if 0 <= nc < 100 and ladder[r][nc] == 1 and visited[r][nc] != 1:
#             dfs(r, nc)
#             return
#     if 0 <= r - 1 < 100 and ladder[r - 1][c] == 1 and visited[r - 1][c] != 1:
#         dfs(r - 1, c)
#         return
#
#
# for tc in range(1, 11):
#     N = int(input())
#     ladder = [tuple(map(int, input().split())) for _ in range(100)]
#     ans = 0
#     visited = [[0] * 100 for _ in range(100)]
#     for y in range(100):
#         if ladder[99][y] == 2:
#             dfs(99, y)
#     print(f'#{tc} {ans}')


def dfs(r, c):
    global ans
    ladder[r][c] = 0
    if r == 0:
        ans = c
        return
    dc = [-1, 1]
    for i in range(2):
        nc = c + dc[i]
        if 0 <= nc < 100 and ladder[r][nc] == 1:
            dfs(r, nc)
            return
    if 0 <= r - 1 < 100 and ladder[r - 1][c] == 1:
        dfs(r - 1, c)
        return


for tc in range(1, 11):
    N = int(input())
    ladder = [list(map(int, input().split())) for _ in range(100)]
    ans = 0
    for y in range(100):
        if ladder[99][y] == 2:
            dfs(99, y)
    print(f'#{tc} {ans}')
