def count(cards):
    o = len(cards)
    for i in range(0, o, 3):  # 문자열은 무조건 SXYSXY... 형태로 들어온다. 3칸씩 건너뛴다.

        s, x, y = cards[i], cards[i+1], cards[i+2]

        num = int(x) * 10 + int(y)  # 문양 뒤의 문자열을 정수로 만들어준다.

        if card[S[s]][num]:  # 이미 가지고 있는 카드면?
            return ['ERROR']        # 에러. 함수 종료.

        card[S[s]][num] = 1  # 에러가 아니면 한 장 추가. 딕셔너리로 문양에 인덱스를 부여해두었다.

    lack = [0, 0, 0, 0]  # 부족분
    for i in range(4):
        for j in range(1, 14):
            lack[i] += (card[i][j] == 0)  # 부족분[문양] += (카드가 없으면 1, 있으면 0)

    return lack


for tc in range(1, int(input())+1):
    order = input()

    S = {'S': 0, 'D': 1, 'H': 2, 'C': 3}  # 카드 문양의 순서

    card = [[0]*14 for _ in range(4)]     # 지금 가지고 있는 커드들을 문양별로 정리해보자

    ans = count(order)  # 카드 정리

    print(f'#{tc}', *ans)
