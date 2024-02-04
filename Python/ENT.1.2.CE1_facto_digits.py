import sys
sys.setrecursionlimit(100_000)


def facto_dig(n, ans):
    global dig
    if dig == 1000:
        print(n - 1)
        return
    elif ans // 1e10 > 0:
        dig += 1
        # print(ans)
        return facto_dig(n + 1, ans * (n + 1) / 1e10)
    else:
        # print(ans)
        return facto_dig(n + 1, ans * (n + 1))


dig = 0
facto_dig(1, 1)
