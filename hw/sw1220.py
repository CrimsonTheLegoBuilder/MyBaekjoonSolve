import sys
sys.stdin = open('input_magnetic.txt', 'r')

for tc in range(1, 11):
    N = int(input())
    desk = [list(map(int, input().split())) for _ in range(N)]
    ans = 0
    for j in range(N):
        p = 2  # 초기 극성 pole은 S로 설정
        for i in range(N):
            if desk[i][j] == 1:  # N을 찾으면 1로 바꿈
                p = 1
            if p == 1 and desk[i][j] == 2:  # N극을 만난 후 S를 만나면?
                p = 2                       # 일단 N 뒤에 S가 있음을 기록
                ans += 1                    # 교착 상태 하나 추가
    print(f'#{tc} {ans}')
