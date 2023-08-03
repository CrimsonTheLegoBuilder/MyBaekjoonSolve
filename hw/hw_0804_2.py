t = int(input())


def balloon(r, c):
    dr = (0, -1, 0, 1)
    dc = (1, 0, -1, 0)
    total = arr[r][c]
    for x in range(4):
        nr = r + dr[x]
        nc = c + dc[x]
        if 0 <= nr < n and 0 <= nc < m:
            total += arr[nr][nc]
    return total


for tc in range(1, 1 + t):
    n, m = map(int, input().split())
    arr = [tuple(map(int, input().split())) for _ in range(n)]
    ans = -1
    for i in range(n):
        for j in range(m):
            if ans < balloon(i, j):
                ans = balloon(i, j)
    print(f'#{tc} {ans}')
