def en_queue(x):       # 삽입
    global rear, front
    if (rear+1) % (n+1) == front:
        return
    rear += 1
    rear %= (n+1)
    queue[rear] = x


def de_queue():       # popleft()
    global rear, front
    if rear == front:
        return
    front += 1
    front %= (n+1)
    return queue[front]


for tc in range(1, int(input())+1):
    n, m = map(int, input().split())
    pizza = list(map(int, input().split()))

    queue = [0] * (n+1)  # 원형 큐
    rear = front = 0     # 초기화

    for i in range(n):   # 초기에 피자 채우기
        en_queue(i)

    next_ = n            # 다음에 넣을 피자

    while True:   # 다 녹아서 빼면 중지
        temp = de_queue()  # 임시로 꺼내본 피자
        if temp is None:
            break
        pizza[temp] //= 2  # 반 녹음

        if not pizza[temp]:      # 다 녹았다면?
            if next_ < m:        # 다음에 넣을 피자가 있다면?
                en_queue(next_)  # 해당 피자를 넣음
                next_ += 1       # 다음 피자 대기
        else:
            en_queue(temp)       # 덜 녹았으면 다시 넣기
        # print(queue)

    print(f'#{tc} {queue[front]+1}')
