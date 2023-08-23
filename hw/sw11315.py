def scan(r, c):
    for x in range(4):
        nr, nc = r, c
        for l in range(4):
            nr += drc[x][0]
            nc += drc[x][1]
            if 0 > nr or N <= nr or 0 > nc or N <= nc:
                break
            if 0 <= nr < N and 0 <= nc < N and board[nr][nc] == '.':
                break
            if l == 3:
                return 1
    return 0


for tc in range(1, int(input())+1):
    stack = []
    N = int(input())
    board = [input() for _ in range(N)]
    drc = [(1, 0), (1, 1), (0, 1), (1, -1)]
    ans = 'NO'
    flag = 0
    for i in range(N):
        for j in range(N):
            if board[i][j] == 'o':
                if scan(i, j):
                    ans = 'YES'
                    flag = 1
                    break
        if flag:
            break
    print(f'#{tc} {ans}')
