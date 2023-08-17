def en_queue(x):       # 삽입
    global rear, front
    # if (rear+1) % (n+1) == front:
    #     return
    rear += 1
    rear %= 9
    queue[rear] = x


def de_queue():       # popleft()
    global rear, front
    # if rear == front:
    #     return
    front += 1
    front %= 9
    return queue[front]


for tc in range(1, 11):
    _ = input()
    queue = [0] + list(map(int, input().split()))
    rear = 8
    front = 0
    # print(queue)
    i = 0
    while True:       # 다 돌면 중지
        i %= 5
        i += 1
        a = de_queue()-i
        if a < 0:
            a = 0
        en_queue(a)
        if queue[rear] <= 0:
            break

    print(f'#{tc}', end=' ')
    for i in range(front+1, front+9):
        print(queue[i%9], end=' ')
    print()
