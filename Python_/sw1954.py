di = (0, 1, 0, -1)
dj = (1, 0, -1, 0)


for tc in range(1, int(input())+1):
    N = int(input())
    arr = [[0]*N for _ in range(N)]
    x = 2
    arr[0][0] = 1
    i, j, s = 0, 0, 0
    while x <= N**2:
        flag = 0
        ni = i + di[s]
        nj = j + dj[s]
        if 0 <= ni < N and 0 <= nj < N and not arr[ni][nj]:
            arr[ni][nj] = x
            x += 1
            i += di[s]
            j += dj[s]
        else:
            s += 1
            s %= 4

    print(f'#{tc}')
    for row in arr:
        print(*row)


# for tc in range(1, int(input())+1):
#     N = int(input())
#     arr = [[0]*N for _ in range(N)]
#     visited = [[0]*N for _ in range(N)]
#     x = 2
#     arr[0][0] = 1
#     visited[0][0] = 1
#     i, j, s = 0, 0, 0
#     while x <= N**2:
#         flag = 0
#         for k in range(4):
#             ni = i + di[(k+s) % 4]
#             nj = j + dj[(k+s) % 4]
#             if 0 <= ni < N and 0 <= nj < N and not visited[ni][nj]:
#                 arr[ni][nj] = x
#                 visited[ni][nj] = 1
#                 i, j = ni, nj
#                 break
#             flag = 1
#         if flag:
#             s += 1
#         x += 1
#
#     print(f'#{tc}')
#     for row in arr:
#         print(*row)
