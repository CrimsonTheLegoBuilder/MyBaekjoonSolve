from collections import deque


for tc in range(1, int(input())+1):
    N, M, K, A, B = map(int, input().split())  # 접수 창구, 정비 창구, 고객의 수, 대상이 거쳐간 창구 번호들

    if N == 1:
        a1 = [[int(input())], [0] * N, [0] * N]  # 접수 창구가 하나일 때와 여러개일 때를 구분해서 받기
    else:
        a1 = [list(map(int, input().split())), [0]*N, [0]*N]

    if M == 1:
        b1 = [[int(input())], [0] * M, [0] * M]  # 정비 창구도 하나일 때와 여러개일 때를 구분한다.
    else:
        b1 = [list(map(int, input().split())), [0]*M, [0]*M]

    t = list(map(int, input().split()))  # 고객이 다녀간 시간 목록

    wait1 = deque()  # 접수 창구 대기열 1
    wait2 = deque()  # 정비 창구 대기열 2
    client = 0       # 입장할 고객 번호
    cur = -1         # 시간
    last = 0         # while문 종료 조건. 고객이 정비 창구를 들어올 때마다 +1

    record = [[0, 0] for _ in range(K+1)]  # 답이 기록될 판

    while True:
        cur += 1       # 1초가 흘렀다.
        if last == K:  # 방금 들어온 손님이 마지막이라면?
            break      # 시뮬레이션 종료

        while client < K and t[client] == cur:  # 현 시각에 방문한 고객이 있다면?
            wait1.append(client+1)              # 대기열 1에 줄을 세운다
            client += 1                         # 다음 손님이 방문한 시간을 본다.

        for i in range(N):                  # 접수 창구부터 시간이 흐른다.
            if a1[2][i]:                    # 창구에 손님이 앉아있다면?
                a1[1][i] += 1               # 해당 창구의 시간이 흐른다.
                if a1[1][i] == a1[0][i]:    # 창구의 접수 처리 시간 만큼 흘렀다면?
                    wait2.append(a1[2][i])  # 대기열 2로 이동.
                    a1[1][i] = 0            # 창구가 비었다.
                    a1[2][i] = 0            # 시간도 초기화.
            if a1[2][i] == 0 and wait1:     # 해당 창구가 비었다면?
                x = wait1.popleft()         # x번 고객님 i번 창구로 오세요~~
                a1[2][i] = x                # x번 고객은 i번 창구에 앉았음
                record[x][0] = i+1          # 해당 고객의 접수 창구 방문 기록

        for j in range(M):                # 정비 창구의 시간이 흐른다.
            if b1[2][j]:                  # 창구에 손님이 앉아있다면?
                b1[1][j] += 1             # 해당 창구의 시간이 흐른다.
                if b1[1][j] == b1[0][j]:  # 창구의 정비 시간 만큼 흘렀다면?
                    b1[1][j] = 0          # 집으로 감 ㅋ
                    b1[2][j] = 0          # 시간도 초기화.
            if b1[2][j] == 0 and wait2:   # 해당 창구가 비었다면?
                x = wait2.popleft()       # x번 고객님 j번 창구로 오세요~~
                last += 1                 # 정비 창구를 방문한 고객의 수 +1
                b1[2][j] = x              # x번 고객은 j번 창구에 앉았음
                record[x][1] = j+1        # 해당 고객의 정비 창구 방문 기록

    ans = 0  # 답 초기화

    for k in range(1, K+1):
        if record[k][0] == A and record[k][1] == B:  # 해당 번호의 창구를 방문한 고객들 찾기
            ans += k

    print(f'#{tc} {ans - (ans<=0)}')  # {고객 번호의 합 - (고객 번호 합이 0이면 1)}

    # print(a1)
    # print(b1)
    # print(client)
    # print(wait2)
    # print(record)

'''
1
4 1 10 3 1
4 6 4 8
1
0 3 4 4 5 6 9 9 9 10

'''