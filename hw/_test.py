def dfs(i, n):
    if i == n:
        print(p)
        return
    else:
        for j in range(n):
            if used[j] == 0:
                p[i] = card[j]
                used[j] = 1
                dfs(i+1, n)
                used[j] = 0


card = list(map(int, input().split()))
used = [0] * 6
p = [0] * 6
dfs(0, 6)
