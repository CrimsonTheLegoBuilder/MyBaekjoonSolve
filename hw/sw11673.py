def reflect(r, c, s):
    # r, c는 좌표, s는 빛의 방향

    global ans
    # 내 자리 컴터 진짜 불량인가봐요 같은 코드인데 실행시간이랑 메모리 너무 다름 ㅠㅠ

    while True:
        r += delta[s][0]
        c += delta[s][1]
        # 방향 함수의 s번째 지시대로 나아감

        if r < 0 or r >= N or c < 0 or c >= N:
            return ans
            # 밖으로 나가면 종료

        mirror = grid[r][c]
        # 거울을 만나면 방향을 본다.

        # 거울이 1 (거울면이 왼쪽 위로 향할 때): 0 - 3, 1 - 2 끼리 반사됨
        if mirror == 1:
            ans += 1
            s = 3 - s

        # 거울이 2 (거울면이 오른쪽 위로 향할 때): 0 - 1, 2 - 3 끼리 반사됨
        if mirror == 2:
            ans += 1
            s = s - 2*(s % 2 != 0) + 1

            # s = s + (2로 나눈 나머지가 있을 때 -1, 없을 때 +1)
            # 헷갈리면 조건문을 여러번 쓰면 된다. 나는 조건문을 싫어해서 모든 상황에서 일반화되는 식을 찾을 뿐임.


for tc in range(1, int(input())+1):
    N = int(input())
    grid = [tuple(map(int, input().split())) for _ in range(N)]
    delta = ((1, 0), (0, 1), (-1, 0), (0, -1))
    # 각각의 방향에 인덱스를 부여.
    # 아래 방향은 0, 오른쪽 방향은 1, 위쪽 방향은 2, 왼쪽 방향은 3
    # 거울은 방향을 바꾸는 역할을 하며, 각각 짝이 있다.
    # 거울이 1 (거울면이 왼쪽 위로 향할 때): 0 - 3, 1 - 2
    # 거울이 2 (거울면이 오른쪽 위로 향할 때): 0 - 1, 2 - 3
    ans = 0
    reflect(0, 0, 1)  # 초기 방향은 1
    print(f'#{tc} {ans}')


# def reflect(r, c):
#     ans = 0
#     dr, dc = 0, 1
#     while True:
#         r += dr
#         c += dc
#         if r < 0 or r >= N or c < 0 or c >= N:
#             return ans
#         if grid[r][c] == 1:
#             ans += 1
#             dr, dc = -dc, -dr
#         if grid[r][c] == 2:
#             ans += 1
#             dr, dc = dc, dr
#
#
# for tc in range(1, int(input())+1):
#     N = int(input())
#     grid = [tuple(map(int, input().split())) for _ in range(N)]
#     print(f'#{tc} {reflect(0, 0)}')
