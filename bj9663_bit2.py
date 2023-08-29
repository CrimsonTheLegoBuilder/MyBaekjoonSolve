import sys


def dfs(x, r, d1, d2):
    global n_q
    if x == N:
        n_q += 1
        return
    d1 <<= 1
    d2 >>= 1
    q_r = r | d1 | d2
    for i in range(N):
        if not q_r & 1 << i:
            dfs(x+1, r | 1 << i, d1 | 1 << i, d2 | 1 << i)


N = int(sys.stdin.readline().strip())
n_q = 0
dfs(0, 0, 0, 0)
print(n_q)
