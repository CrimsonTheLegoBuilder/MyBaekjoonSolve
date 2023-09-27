import sys


t = int(sys.stdin.readline())

for _ in range(t):
    n = int(sys.stdin.readline())
    arr = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
    temp = [0] * n
    min_ = 1e10

    def combi(depth, idx):
        global min_
        if depth == n // 2:
            ans = [0, 0]
            for j in range(n):
                if temp[j] == 1:
                    ans[0] += arr[j][0]
                    ans[1] += arr[j][1]
                else:
                    ans[0] -= arr[j][0]
                    ans[1] -= arr[j][1]
            v = (ans[0] ** 2 + ans[1] ** 2) ** 0.5
            min_ = min(min_, v)
            return
        for i in range(idx, n):
            if not temp[i]:
                temp[i] = 1
                combi(depth + 1, i + 1)
                temp[i] = 0

    combi(0, 0)
    print(format(min_, '.10f'))
