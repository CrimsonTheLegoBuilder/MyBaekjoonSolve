for tc in range(int(input())):
    N, *T = map(int, input().split())
    T += [0]
    dp = [0] * N
    for i in range(N-2, -1, -1):
        tmp = int(12e12)
        for j in range(i+T[i], i, -1):
            if j < N:
                tmp = min(tmp, dp[j])
        dp[i] = tmp + 1
    # print(T)
    # print(dp)
    ans = min(dp[:T[0]+1])
    print(f"#{tc+1} {ans}")
