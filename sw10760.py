def scan(r, c):
    cnt = 0
    cur = moon[r][c]
    for i in range(8):
        nr = r + dr[i]
        nc = c + dc[i]
        if 0 <= nr < n and 0 <= nc < m and moon[nr][nc] < cur:
            cnt += 1
    # print(cnt)
    return 1 if cnt > 3 else 0


for t in range(1, int(input())+1):
    n, m = map(int, input().split())
    moon = [tuple(map(int, input().split())) for _ in range(n)]
    dr = (1, 1, -1, -1, 0, 1, 0, -1)
    dc = (1, -1, -1, 1, 1, 0, -1, 0)
    ans = 0
    for x in range(n):
        for y in range(m):
            ans += scan(x, y)
    print(f'#{t} {ans}')
