def play(order, bonus):    # 게임을 시작하지.
    for o in order:        # 덱 A 로부터 명령을 읽어들임
        if o.isnumeric():  # 숫자네?
            num = int(o) + bonus     # 보너스와 더하고 홀짝 판정
            if num % 2:
                queue_B.append(num)  # 홀수면 B 에다가 넣기
            else:
                stack_C.append(num)  # 짝수면 C 에다가 넣기
        else:
            bonus += 1     # 뽀나스 올라감 ㅋ


t = int(input())
for tc in range(1, t+1):
    n, m = map(int, input().split())
    card_A = list(map(str, input().split()))  # 덱 A 생성
    queue_B = []  # B는 꾸에우에
    stack_C = []  # C는 스따끄
    b, c = 0, 0   # 카드 정리가 끝난 후 꺼낼 b와 c는 0으로 초기화
    ans = 0       # 싸피야 도박은 나쁜거야...
    bonus_ = 0    # 뽀나쓰
    play(card_A, bonus_)  # 게임을 시작하지.
    for x in range(1, m+1):
        if queue_B:
            b = queue_B.pop(0)  # 밑장 빼기
        if stack_C:
            c = stack_C.pop()   # pop pop pop ~~~!
        if x == m:       # 김싸피의 turn
            ans = b + c  # 김싸피가 챙겨갈 점수. 빵점일수도 있음
        b, c = 0, 0      # 매 턴 꺼낸 b와 c는 0으로 초기화
    print(f'#{tc} {ans}')
