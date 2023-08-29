import sys


def dfs(x, r, d1, d2):
    global n_queen
    if x == N:
        n_queen += 1
        return
    d1 >>= 1
    d2 <<= 1
    attacked = r | d1 | d2

    for i in range(N):
        if not (attacked & 1 << i):
            dfs(x+1, r | 1 << i, d1 | 1 << i, d2 | 1 << i)


N = int(sys.stdin.readline().strip())
n_queen = 0
dfs(0, 0, 0, 0)
print(n_queen)
