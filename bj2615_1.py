import sys

board = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(19)]
row = (-1, 0, 1, 1)  # CCW
col = (1, 1, 1, 0)


def find_winner(x, y):
    color = board[x][y]

    for i in range(4):
        idx_x = x + row[i]
        idx_y = y + col[i]
        cnt = 1

        while 0 <= idx_x < 19 and 0 <= idx_y < 19 and board[idx_x][idx_y] == color:
            cnt += 1

            if cnt == 5:
                if 0 <= x - row[i] < 19 and 0 <= y - col[i] < 19 and board[x - row[i]][y - col[i]] == color:
                    break
                if 0 <= idx_x + row[i] < 19 and 0 <= idx_y + col[i] < 19 and board[idx_x + row[i]][idx_y + col[i]] == color:
                    break
                print(color)
                print(x + 1, y + 1)
                exit(0)

            idx_x += row[i]
            idx_y += col[i]
    return 0


for j in range(19):
    for k in range(19):
        if board[j][k] != 0:
            find_winner(j, k)

print(0)
