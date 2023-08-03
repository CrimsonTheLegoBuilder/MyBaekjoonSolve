import sys
sys.stdin = open('input (8).txt')


def find_m(d1, d2):
    global ans
    dr = (1, 0)  # CCW
    dc = (0, 1)
    r, c = d1, d2

    for i in range(2):
        cnt = 1
        nr = r + dr[i]
        nc = c + dc[i]
        while 0 <= nr < n and 0 <= nc < n and board[nr][nc] == 1:
            cnt += 1
            if cnt == m:
                if ((r - dr[i] < 0 or c - dc[i] < 0) |
                    (0 <= r - dr[i] < n and 0 <= c - dc[i] < n and board[r - dr[i]][c - dc[i]] != 1)) & \
                        ((nr + dr[i] > n - 1 or nc + dc[i] > n - 1) |
                         (0 <= nr + dr[i] < n and 0 <= nc + dc[i] < n and board[nr + dr[i]][nc + dc[i]] != 1)):
                    ans += 1
                break
            nr += dr[i]
            nc += dc[i]
    return


t = int(input())
for tc in range(1, t+1):
    n, m = map(int, input().split())
    board = [tuple(map(int, input().split())) for _ in range(n)]
    ans = 0
    for j in range(n):
        for k in range(n):
            if board[j][k] == 1:
                find_m(j, k)
    print(f'#{tc} {ans}')

'''
1
5 3
0 0 1 1 1
1 1 1 1 0
0 0 1 0 0
0 1 1 1 1
1 1 1 0 1
'''


