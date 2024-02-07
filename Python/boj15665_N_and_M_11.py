import sys
sys.setrecursionlimit(10000)


def dfs():
    if len(ans) == M:
        print(*ans)
        return
    pre_num = -1
    for i in range(N):
        if pre_num != arr[i]:
            pre_num = arr[i]
            ans.append(arr[i])
            dfs()
            ans.pop()


N, M = map(int, sys.stdin.readline().strip().split())
arr = sorted(list(map(int, sys.stdin.readline().strip().split())))
ans = []
dfs()
