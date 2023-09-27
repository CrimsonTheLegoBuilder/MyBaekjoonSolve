t = int(input())

for i in range(1, t + 1):
    n, m = map(int, input().split())
    fly = [list(map(int, input().split())) for _ in range(n)]

    ti = [1, 0, -1, 0]
    tj = [0, 1, 0, -1]  # X축 +방향부터 CCW ROTATE

    xi = [1, -1, -1, 1]
    xj = [1, 1, -1, -1]  # 1사분면부터 CCW ROTATE

    answer = 0

    for j in range(n):
        for k in range(n):
            cnt_t = fly[j][k]  # 첫 칸 값 할당
            cnt_x = fly[j][k]  # 첫 칸 값 할당
            for p in range(4):
                for q in range(1, m):
                    a = q * ti[p] + j
                    b = q * tj[p] + k

                    c = q * xi[p] + j
                    d = q * xj[p] + k

                    if 0 <= a < n and 0 <= b < n:  # 범위 확인 후 넘어가지 않는 것들만 더하기
                        cnt_t += fly[a][b]
                    if 0 <= c < n and 0 <= d < n:  # 범위 확인 후 넘어가지 않는 것들만 더하기
                        cnt_x += fly[c][d]

            if answer < cnt_x:
                answer = cnt_x
            if answer < cnt_t:
                answer = cnt_t

    print('#' + str(i) + ' ' + str(answer))
