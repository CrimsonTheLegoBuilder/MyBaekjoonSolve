def shot(r, c):
    total = 0
    for o in range(8):
        i, j, rabbit = r, c, 0  # 각 방향에 대해 시작점 i, j, 잡은 토끼 수 rabbit 초기화
        while True:
            i += drc[o][0]  # 한 방향으로 계속 나아가면서 훑어본다.
            j += drc[o][1]

            if 0 > i or i >= N or 0 > j or j >= N:  # 총알이 끝까지 갔다면?
                total += rabbit
                break            # 이 방향에서 잡은 토끼를 기록하고 넘어감

            if grid[i][j] == 3:
                total += rabbit
                break            # 바위를 만나도 마찬가지

            if grid[i][j] == 2:
                rabbit += 1      # 토끼를 지나갈 때마다 추가

            if grid[i][j] == 1:
                break            # 사냥꾼을 쏠 수는 없다.
    return total


for tc in range(1, int(input())+1):
    N = int(input())
    grid = [tuple(map(int, input().split())) for _ in range(N)]

    drc = ((1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1))
    # 8방향 탐색

    ans = 0
    for x in range(N):
        for y in range(N):
            if grid[x][y] == 1:  # 사냥꾼이 보이면 함수 실행
                ans += shot(x, y)
    print(f'#{tc} {ans}')
