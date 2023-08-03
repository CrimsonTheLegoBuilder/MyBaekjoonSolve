def dfs1(x, r, arr):
    global ans
    if len(comb) == 2:
        n = comb[1] - comb[0] + 1
        for o in range(n//2):
            if arr[r][comb[0] + o] != arr[r][comb[1] - o]:
                return
        if ans < n:
            ans = n
        return
    for k in range(x, 100):
        comb.append(k)
        dfs1(k + 1, r, arr)
        comb.pop()
    return


def dfs2(x, r, arr):
    global ans
    if len(comb) == 2:
        n = comb[1] - comb[0] + 1
        for o in range(n//2):
            if arr[comb[0] + o][r] != arr[comb[1] - o][r]:
                return
        if ans < n:
            ans = n
        return
    for k in range(x, 100):
        comb.append(k)
        dfs2(k + 1, r, arr)
        comb.pop()
    return


for tc in range(1, 11):
    arr1 = []
    ans = 0
    _ = input()
    comb = []
    for _ in range(100):
        temp = list(input())
        arr1.append(temp)
    for i in range(100):
        comb.clear()
        dfs1(0, i, arr1)
        comb.clear()
        dfs2(0, i, arr1)
    print(f'#{tc} {ans}')
