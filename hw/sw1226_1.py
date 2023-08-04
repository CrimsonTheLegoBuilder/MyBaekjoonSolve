# 미로1

def maze_srch(input_board):
    startPoint = (1, 1)  # 시작지점 설정

    r, c = startPoint
    visited_coord = [startPoint]

    while visited_coord:

        dead_end = True
        for d in range(4):

            nr = r + dr[d]
            nc = c + dc[d]

            if input_board[nr][nc] != 1 and (nr, nc) != visited_coord[-1]:
                dead_end = False

                if input_board[nr][nc] == 3:
                    return 1

                visited_coord.append((r, c))
                r = nr
                c = nc
                break

        if dead_end:
            # if input_board[r-1][c] and input_board[r][c+1] \
            #     and input_board[r+1][c] and input_board[r][c-1]:
            #     return 0
            input_board[r][c] = 1
            r, c = visited_coord.pop()

    return 0


for i in range(10):
    T = int(input())

    board = [list(map(int, list(input()))) for _ in range(16)]

    dr = [-1, 0, 1, 0]
    dc = [0, 1, 0, -1]

    print(f'#{T} {maze_srch(board)}')