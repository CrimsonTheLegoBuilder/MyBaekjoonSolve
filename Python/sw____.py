def scan(r, c):
    cnt = 0
    cur = mountain[r][c]
    for i in range(8):
        nr = r + dr[i]
        nc = c + dc[i]
        if 0 <= nr < n and 0 <= nc < n and mountain[nr][nc] < cur:
            cnt += 1
    # print(cnt)
    return cur if cnt == 8 else None


for t in range(1, int(input())+1):
    n = int(input())
    mountain = [tuple(map(int, input().split())) for _ in range(n)]
    dr = (1, 1, -1, -1, 0, 1, 0, -1)
    dc = (1, -1, -1, 1, 1, 0, -1, 0)
    high = 0
    low = int(12e12)
    top = 0
    for x in range(n):
        for y in range(n):
            temp = scan(x, y)
            # print(temp)
            if temp:
                top += 1
                if high < temp:
                    high = temp
                if low > temp:
                    low = temp
    # print(top)
    if top > 1:
        print(f'#{t} {high - low}')
    else:
        print(f'#{t} -1')
