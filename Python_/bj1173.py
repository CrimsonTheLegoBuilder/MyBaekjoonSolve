N, m, M, T, R = map(int, input().split())
temp = m
cnt = 0
cnt1 = 0

if M - m < T:
    print(-1)
else:
    while cnt1 < N:
        if temp + T <= M:
            temp += T
            cnt1 += 1
            cnt += 1
        else:
            while temp + T > M:
                temp -= R
                if temp < m:
                    temp = m
                cnt += 1

    print(cnt)
    print(cnt1)
