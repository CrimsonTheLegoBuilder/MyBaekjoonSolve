# 임솔이 누님 코드 조금 더 수정
for t in range(1, int(input())+1):
    N = int(input())
    arr = [list(input()) for _ in range(N)]
    ai = (1, 0, -1, 0)
    aj = (0, -1, 0, 1)
    dic = {'A': 1, 'B': 2, 'C': 3}
    for i in range(N):
        for j in range(N):
            if arr[i][j] == 'A' or arr[i][j] == 'B' or arr[i][j] == 'C':
                for k in range(4):
                    for l in range(1, dic[arr[i][j]] + 1):
                        ni = i + ai[k] * l
                        nj = j + aj[k] * l
                        if 0 <= ni < N and 0 <= nj < N and arr[ni][nj] == 'H':
                            arr[ni][nj] = 'X'

    count = 0
    for i in range(N):
        for j in range(N):
            if arr[i][j] == 'H':
                count += 1
    print(f'#{t} {count}')
