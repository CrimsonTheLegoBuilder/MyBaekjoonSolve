def is_omok():
    for i in range(N):
        for j in range(N):
            for k in range(8):
                cnt = 0
                for m in range(5):
                    ni = i + di[k] * m
                    nj = j + dj[k] * m
                    if 0 <= ni < N and 0 <= nj < N:
                        if arr[ni][nj] == 'o':
                            cnt += 1
                            if cnt >= 5:
                                print(f'#{test_case} YES')
                                return
                        else:
                            break

    print(f'#{test_case} NO')
    return


di = [-1, 0, 1, 1, 1, 0, -1, -1]
dj = [1, 1, 1, 0, -1, -1, -1, 0]

T = int(input())
for test_case in range(1, T + 1):
    N = int(input())
    arr = [input() for _ in range(N)]
    is_omok()