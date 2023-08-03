def bubble_sort():
    for i in range(N - 1, -1, -1):
        for j in range(i):
            if colors[j][4] > colors[j + 1][4]:
                colors[j], colors[j + 1] = colors[j + 1], colors[j]


def coloring(arr):
    for i in range(arr[0], arr[2] + 1):
        for j in range(arr[1], arr[3] + 1):
            if board[i][j] == 0:
                board[i][j] = arr[4]
            elif board[i][j] != 0:
                board[i][j] = 3


t = int(input())
for tc in range(1, t+1):
    N = int(input())
    colors = [tuple(map(int, input().split())) for _ in range(N)]
    board = [[0] * 10 for _ in range(10)]
    bubble_sort()
    for x in colors:
        coloring(x)
    ans = 0
    for x in board:
        for three in x:
            if three == 3:
                ans += 1
    print(f'#{tc} {ans}')
