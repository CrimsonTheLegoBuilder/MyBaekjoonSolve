def snail(r, c, x, y):
    arr[r][c] = x
    dr = (0, 1, 0, -1)
    dc = (1, 0, -1, 0)
    for i in range(4):
        nr = r + dr[(i+y) % 4]
        nc = c + dc[(i+y) % 4]
        if 0 <= nr < n and 0 <= nc < n and not arr[nr][nc]:
            y1 = i+y
            snail(nr, nc, x + 1, y1)
    return


for tc in range(1, int(input())+1):
    n = int(input())
    arr = [[0]*n for _ in range(n)]
    snail(0, 0, 1, 0)
    print(f'#{tc}')
    for row in arr:
        print(*row)
