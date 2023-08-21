def dfs(r, c, s):
    global ans
    while True:
        r += delta[s][0]
        c += delta[s][1]
        if r < 0 or r >= N or c < 0 or c >= N:
            return ans
        mirror = grid[r][c]
        if mirror == 1:  # 거울이 1 (거울면이 왼쪽 위로 갈 때): 0 - 3, 1 - 2
            ans += 1
            s = 3 - s
        if mirror == 2:  # 거울이 2 (거울면이 오른쪽 위로 갈 때): 0 - 1, 2 - 3
            ans += 1
            s = s - 2*(s % 2 != 0) + 1  # s + (나머지가 있을 때 -1, 없을 때 +1)


for tc in range(1, int(input())+1):
    N = int(input())
    grid = [tuple(map(int, input().split())) for _ in range(N)]
    stack = []
    delta = ((1, 0), (0, 1), (-1, 0), (0, -1))  # 각각의 상태에 대한 방향.
    # 아래 방향은 0, 오른쪽 방향은 1, 위쪽 방향은 2, 왼쪽 방향은 3
    # 거울은 방향을 바꾸는 역할을 하며, 각각 순서가 있음
    # 거울이 1 (거울면이 왼쪽 위로 갈 때): 0 - 3, 1 - 2
    # 거울이 2 (거울면이 오른쪽 위로 갈 때): 0 - 1, 2 - 3
    ans = 0
    dfs(0, 0, 1)  # 초기 방향은 1
    print(f'#{tc} {ans}')
