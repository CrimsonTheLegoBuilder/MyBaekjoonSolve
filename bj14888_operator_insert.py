import sys
MAX_ = -1e10
MIN_ = 1e10


def four_arith(n, ans):
    global MAX_, MIN_, opt
    if n == N:
        MAX_ = max(MAX_, ans)
        MIN_ = min(MIN_, ans)
    else:
        for x in range(4):
            if opt[x] > 0:
                if x == 0:
                    opt[x] -= 1
                    four_arith(n + 1, ans + arr[n])
                    opt[x] += 1
                elif x == 1:
                    opt[x] -= 1
                    four_arith(n + 1, ans - arr[n])
                    opt[x] += 1
                elif x == 2:
                    opt[x] -= 1
                    four_arith(n + 1, ans * arr[n])
                    opt[x] += 1
                elif x == 3:
                    opt[x] -= 1
                    four_arith(n + 1, int(ans / arr[n]))
                    opt[x] += 1


N = int(input())
arr = list(map(int, sys.stdin.readline().strip().split()))
opt = list(map(int, sys.stdin.readline().strip().split()))

four_arith(1, arr[0])
print(MAX_)
print(MIN_)
