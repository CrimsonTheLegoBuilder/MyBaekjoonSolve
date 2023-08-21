def shot(r, c):
    total = 0
    for o in range(8):
        i, j, temp = r, c, 0
        while True:
            i += drc[o][0]
            j += drc[o][1]
            if 0 > i or i >= N or 0 > j or j >= N:
                total += temp
                break
            if grid[i][j] == 3:
                total += temp
                break
            if grid[i][j] == 2:
                temp += 1
            if grid[i][j] == 1:
                break
    return total


for tc in range(1, int(input())+1):
    N = int(input())
    grid = [tuple(map(int, input().split())) for _ in range(N)]
    drc = ((1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1))
    ans = 0
    for x in range(N):
        for y in range(N):
            if grid[x][y] == 1:
                ans += shot(x, y)
    print(f'#{tc} {ans}')


# if 0 <= i < N and 0 <= j < N:
