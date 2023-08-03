t = int(input())


def four_diff(r, c):
    dr = (0, -1, 0, 1)
    dc = (1, 0, -1, 0)
    total = 0
    for x in range(4):
        nr = r + dr[x]
        nc = c + dc[x]
        if 0 <= nr < n and 0 <= nc < n:
            if arr[nr][nc] > arr[r][c]:
                total += arr[nr][nc] - arr[r][c]
            else:
                total += arr[r][c] - arr[nr][nc]
    return total


for tc in range(1, 1 + t):
    n = int(input())
    arr = [tuple(map(int, input().split())) for _ in range(n)]
    ans = 0
    for i in range(n):
        for j in range(n):
            ans += four_diff(i, j)
    print(f'#{tc} {ans}')
