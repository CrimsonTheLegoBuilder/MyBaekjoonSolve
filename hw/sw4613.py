for tc in range(1, int(input())+1):
    N, M = map(int, input().split())
    tricolor = [input() for _ in range(N)]
    ans = int(12e12)
    for i in range(N-2):
        for j in range(i+1, N-1):
            temp = 0
            for k in range(i+1):
                for l in range(M):
                    if tricolor[k][l] != 'W':
                        temp += 1
            for k in range(i+1, j+1):
                for l in range(M):
                    if tricolor[k][l] != 'B':
                        temp += 1
            for k in range(j+1, N):
                for l in range(M):
                    if tricolor[k][l] != 'R':
                        temp += 1
            ans = min(ans, temp)
    print(f'#{tc} {ans}')
