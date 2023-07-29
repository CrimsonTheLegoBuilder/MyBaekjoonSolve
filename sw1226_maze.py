def find_user(arr, x):
    ans = None
    for i in range(len(arr)):
        if x in arr[i]:
            ans = i, arr[i].index(x)
    return ans


def find_path(board, p):
    r, c = p  # start point
    dr = (-1, 0, 1, 0)  # CW
    dc = (0, 1, 0, -1)
    visited_node = [p]

    while visited_node:
        flag = True  # flag become False when all 4 direction blocked

        for i in range(4):  # look in all 4 direction
            nr = r + dr[i]
            nc = c + dc[i]
            if board[nr][nc] != 1 and (nr, nc) != visited_node[-1]:  # if forwardable path exists:
                flag = False
                if board[nr][nc] == 3:
                    return 1
                visited_node.append((r, c))  # record current location
                r, c = nr, nc  # move forward
                break

        if flag:
            board[r][c] = 1
            r, c = visited_node.pop()  # move backward
    return 0


for j in range(1):
    _ = input()
    maze = [list(map(int, list(input()))) for _ in range(16)]
    # print(maze)
    player = find_user(maze, 2)
    # print(player)
    print(f'#{j} {find_path(maze, player)}')
