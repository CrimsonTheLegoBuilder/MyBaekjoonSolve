def cover(r, c):
    t = town[r][c]
    for x in range(1, towers[t]+1):  # 기지국의 종류에 따라 범위가 달라짐
        for o in range(4):
            nr = r + drc[o][0] * x
            nc = c + drc[o][1] * x
            if 0 <= nr < N and 0 <= nc < N and town[nr][nc] == 'H':
                town[nr][nc] = 'X'  # 커버되는 곳은 전부 X로 바꿔버리기


for tc in range(1, int(input())+1):
    towers = {'A': 1, 'B': 2, 'C': 3}  # 기지국의 종류
    N = int(input())
    drc = ((1, 0), (0, 1), (-1, 0), (0, -1))  # 방향키
    town = [list(input()) for _ in range(N)]
    for i in range(N):
        for j in range(N):
            if town[i][j] == 'H':  # 집은 함수를 실행하지 않음
                continue
            if town[i][j] != 'X':  # 집도 아니고 아무것도 아닌 곳도 아니면 기지국이겠지
                cover(i, j)        # 가즈아
    ans = 0
    for i in range(N):
        for j in range(N):
            if town[i][j] == 'H':  # 다 커버하고도 남은 집은 어쩌냐...
                ans += 1
    print(f'#{tc} {ans}')
