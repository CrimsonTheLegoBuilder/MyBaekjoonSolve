from collections import deque


for tc in range(1, int(input())+1):
    N, M, K, A, B = map(int, input().split())
    if N == 1:
        a1 = [[int(input())], [0] * N, [0] * N]
    else:
        a1 = [list(map(int, input().split())), [0]*N, [0]*N]
    if M == 1:
        b1 = [[int(input())], [0] * M, [0] * M]
    else:
        b1 = [list(map(int, input().split())), [0]*M, [0]*M]
    t = list(map(int, input().split()))
    wait1 = deque()
    wait2 = deque()
    client = 0
    cur = -1
    last = 0
    record = [[0, 0] for _ in range(K+1)]
    while True:
        cur += 1
        if last == K:
            break
        while client < K and t[client] == cur:
            wait1.append(client+1)
            client += 1
        for i in range(N):
            if a1[2][i]:
                a1[1][i] += 1
                if a1[1][i] == a1[0][i]:
                    wait2.append(a1[2][i])
                    a1[1][i] = 0
                    a1[2][i] = 0
            if a1[2][i] == 0 and wait1:
                temp = wait1.popleft()
                a1[2][i] = temp
                record[temp][0] = i+1
        for j in range(M):
            if b1[2][j]:
                b1[1][j] += 1
                if b1[1][j] == b1[0][j]:
                    b1[1][j] = 0
                    b1[2][j] = 0
            if b1[2][j] == 0 and wait2:
                temp = wait2.popleft()
                last += 1
                b1[2][j] = temp
                record[temp][1] = j+1
    ans = 0
    for k in range(1, K+1):
        if record[k][0] == A and record[k][1] == B:
            ans += k
    print(f'#{tc} {ans - (ans<=0)}')

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