import sys
sys.stdin = open('input_magnetic.txt', 'r')

for tc in range(1, 11):
    N = int(input())
    desk = [list(map(int, input().split())) for _ in range(N)]
    ans = 0
    for j in range(N):
        p = 2
        for i in range(N):
            if desk[i][j] == 1:
                p = 1
            if p == 1 and desk[i][j] == 2:
                p = 2
                ans += 1
    print(f'#{tc} {ans}')
