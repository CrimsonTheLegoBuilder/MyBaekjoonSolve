def total(r, c):
    tmp = grid[r][c:c+M]
    # print(tmp)
    ans = 0
    for v in range(1, 1 << M):
        choose = [tmp[x] for x in range(M) if (1 << x) & v]
        if sum(choose) <= C:
            ans = max(ans, sum([x**2 for x in choose]))
            # print(choose, ans)
    return ans


for tc in range(int(input())):
    N, M, C = map(int, input().split())
    grid = [tuple(map(int, input().split())) for _ in range(N)]
    memo = [[0]*(N-M+1) for _ in range(N)]
    for i in range(N):
        for j in range(N-M+1):
            memo[i][j] = total(i, j)
    X = 0
    for i in range(N):
        for j in range(N-M+1):
            tmp1 = memo[i][j]
            for k in range(N):
                for l in range(j, N-M+1):
                    if k == i and j <= l <= j+M-1:
                        continue
                    tmp2 = memo[k][l]
                    X = max(X, tmp1+tmp2)
    print(f'#{tc+1} {X}')

'''
1
10
4 2 13
6 1 9 7
9 8 5 8
3 4 5 3
8 2 6 7

'''