for tc in range(1, int(input())+1):
    pipe = str(input().strip())
    dp = [0]*(len(pipe)+1)
    for i in range(len(pipe)):
        if pipe[i] == '(':
            dp[i+1] = dp[i]+1
        if pipe[i] == ')':
            dp[i+1] = dp[i]-1
    # print(dp)
    cnt = 0
    for i in range(1, len(pipe)):
        if pipe[i] == ')' and pipe[i-1] == '(':
            cnt += dp[i+1]
        if pipe[i] == ')' and pipe[i-1] == ')':
            cnt += 1
    print(f'#{tc} {cnt}')
