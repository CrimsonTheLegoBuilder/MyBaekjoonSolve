def scan(r, c):  # 스캐너 함수
    flag = 1
    cur = mountain[r][c]  # 현재 칸의 높이
    for k in range(4):
        nr = r + dr[k]
        nc = c + dc[k]
        if 0 <= nr < n and 0 <= nc < n and mountain[nr][nc] >= cur:  # 만일 주변 칸 중에 높거나 같은 칸이 있다면?
            flag = 0         # 봉우리가 아님
    return 1 if flag else 0  # 봉우리면 1, 아니면 0


t = int(input())
for tc in range(1, t+1):
    n = int(input())
    mountain = [tuple(map(int, input().split())) for _ in range(n)]
    dr = (0,1,0,-1)
    dc = (1,0,-1,0)
    ans = 0
    for i in range(n):
        for j in range(n):
            ans += scan(i, j)  # 봉우리의 수를 더한다.
    print(f'#{tc} {ans}')
