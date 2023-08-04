def find_start(board):
    start_point = [0, 0]
    for j in range(16):
        if 2 in board[j]:
            start_point[0] = j
            start_point[1] = board[j].index(2)
            return tuple(start_point)


def dfs(board, point):
    result = 0
    # noLonger = []  # 더이상 방문할 필요가 없는 좌표
    # visited.append(coord)  # visited 리스트에 가장 마지막 값이 현위치
    dx = [0, -1, 0, 1]
    dy = [1, 0, -1, 0]  # CCW search
    x, y = point[0], point[1]
    print(x, y)
    head = 0
    while True:
        print(x, y)
        if board[x + dx[head]][y + dy[head]] == 0:
            x += dx[head]
            y += dy[head]
        elif board[x + dx[head]][y + dy[head]] == 1:
            head = (head + 1) % 4
        elif board[x + dx[head]][y + dy[head]] == 3:
            print(1)
            return
        elif board[x + dx[head]][y + dy[head]] == 2:
            print(0)
            return


# for i in range(1, 11):
_ = input()
maze = [list(map(int, input())) for _ in range(16)]
p = find_start(maze)
print(p)
dfs(maze, p)

