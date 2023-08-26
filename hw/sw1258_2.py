import sys
sys.stdin = open('input_magazine.txt', 'r')


def scan(r, c):
    row, col = 0, 0
    nr, nc = r, c
    while nr < N and magazine[nr][c]:
        nr += 1
        row += 1
    while nc < N and magazine[r][nc]:
        nc += 1
        col += 1
    for x in range(r, nr):
        for y in range(c, nc):
            magazine[x][y] = 0
    return row*col, row, col


for tc in range(1, int(input())+1):
    N = int(input())
    magazine = [list(map(int, input().split())) for _ in range(N)]
    sub_matrix = []
    for i in range(N):
        for j in range(N):
            if magazine[i][j]:
                sub_matrix.append(scan(i, j))
    sub_matrix.sort(key=lambda x: (x[0], x[1]))
    print(f'#{tc} {len(sub_matrix)}', end='')
    for _, i, j in sub_matrix:
        print(f' {i} {j}', end='')
    print()

    # ans = []
    # for _, i, j in sub_matrix:
    #     ans. append(i)
    #     ans. append(j)
    # print(f'#{tc}', len(ans)//2, *ans)
