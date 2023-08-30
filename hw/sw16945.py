def dfs(i, n):
    global ans
    if i == n:
        a = sorted(p[:3])
        b = sorted(p[3:])
        a1, a2, a3 = a
        b1, b2, b3 = b
        flag1, flag2 = 0, 0
        if a1 == a2 == a3 or a1+2 == a2+1 == a3:
            flag1 = 1
        if b1 == b2 == b3 or b1+2 == b2+1 == b3:
            flag2 = 1
        if flag1 and flag2:
            ans = 1
        return
    else:
        for j in range(n):
            if used[j] == 0:
                p[i] = card[j]
                used[j] = 1
                dfs(i+1, n)
                used[j] = 0


for tc in range(1, int(input())+1):
    card = list(map(int, input()))
    used = [0] * 6
    p = [0] * 6
    ans = 0
    dfs(0, 6)
    print(f'#{tc} {ans}')
