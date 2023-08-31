for tc in range(1, int(input())+1):
    N = int(input())
    dock = sorted([tuple(map(int, input().split())) for _ in range(N)], key=lambda x: x[1])  # 종료 시간 기준 정렬
    ans = 0
    end = -1  # 시작시간이 언젠지 몰라서 걍 -1 로 처리
    for s, e in dock:  # 시작 시간, 종료 시간
        if end <= s:   # 끝나는 시간 직후에 있는 작업이 있다면?
            end = e    # 다음 작업으로 넘어감
            ans += 1   # 다음 작업의 종료 시간
    print(f'#{tc} {ans}')
