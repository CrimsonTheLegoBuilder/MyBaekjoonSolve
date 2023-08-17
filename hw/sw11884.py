def en_queue(x):       # 삽입
    global rear, front
    # if (rear+1) % (n+1) == front:
    #     return
    rear += 1
    # rear %= (n+1)
    queue[rear] = x


def de_queue():       # popleft()
    global rear, front
    # if rear == front:
    #     return
    front += 1
    # front %= (n+1)
    return queue[front]


for tc in range(1, int(input())+1):
    n, m = map(int, input().split())
    arr = list(map(int, input().split()))

    queue = [0] * (n+m)  # 원형 큐
    rear = front = -1     # 초기화

    for i in range(n):   # 초기에 숫자 채우기
        en_queue(arr[i])
    # print(queue)
    cnt = 0
    while cnt < m:       # 다 돌면 중지
        cnt += 1
        en_queue(de_queue())
    # print(queue)
    print(f'#{tc} {queue[front+1]}')
