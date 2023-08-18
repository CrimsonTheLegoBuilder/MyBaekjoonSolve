# def dfs(r, c):
#     global flag
#     stack.append((r, c))
#     while stack:
#         temp = 1
#         r, c = stack[-1]
#         maze[r][c] = 1
#         # for row in maze:
#         #     print(*row)
#         for k in range(4):
#             nr = r + dr[k]
#             nc = c + dc[k]
#             if 0 <= nr < n and 0 <= nc < n and maze[nr][nc] == 3:
#                 flag = 1
#                 return 1
#             if 0 <= nr < n and 0 <= nc < n and not maze[nr][nc]:
#                 stack.append((nr, nc))
#                 temp = 0
#                 break
#         if temp:
#             stack.pop()
#     return 0
#
#
# for t in range(1, int(input())+1):
#     n = int(input())
#     maze = [list(map(int, list(input()))) for _ in range(n)]
#     stack = []
#     flag = 0
#     dr = (1, 0, -1, 0)
#     dc = (0, 1, 0, -1)
#     for i in range(n):
#         for j in range(n):
#             if maze[i][j] == 2:
#                 dfs(i, j)
#                 print(f'#{t} {dfs(i, j)}')
#     # for row in maze:
#     #     print(*row)
#     # if flag:
#     #     print(f'#{t} 1')
#     # else:
#     #     print(f'#{t} 0')
import sys
sys.stdin = open('input_maze.txt', 'r')


def dfs(r, c):
    stack.append((r, c))
    while stack:
        temp = 1
        r, c = stack[-1]
        maze[r][c] = 1
        for k in range(4):
            nr = r + dr[k]
            nc = c + dc[k]
            if 0 <= nr < 100 and 0 <= nc < 100 and maze[nr][nc] == 3:
                return 1
            if 0 <= nr < 100 and 0 <= nc < 100 and not maze[nr][nc]:
                stack.append((nr, nc))
                temp = 0
                break
        if temp:
            stack.pop()
    return 0


for t in range(1, 11):
    _ = int(input())
    maze = [list(map(int, list(input()))) for _ in range(100)]
    stack = []
    dr = (1, 0, -1, 0)
    dc = (0, 1, 0, -1)
    flag = 0
    for i in range(100):
        for j in range(100):
            if maze[i][j] == 2:
                print(f'#{t} {dfs(i, j)}')
                flag = 1
                break
        if flag:
            break
