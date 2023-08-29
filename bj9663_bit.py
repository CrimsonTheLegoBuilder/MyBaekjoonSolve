import sys


def dfs(x, r, d1, d2):
    global n_queen
    if x == N:
        n_queen += 1
        return
    for i in range(N):
        if not r & 1 << i and not d1 & 1 << (x+i) and not d2 & 1 << (N-1+x-i):
            dfs(x+1, r | 1 << i, d1 | 1 << (x+i), d2 | 1 << (N-1+x-i))


N = int(sys.stdin.readline().strip())
n_queen = 0
dfs(0, 0, 0, 0)
print(n_queen)
