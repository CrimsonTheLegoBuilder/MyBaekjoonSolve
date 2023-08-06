import sys
sys.setrecursionlimit(10000)


def dfs(x):
    if len(ans) == m:
        print(*ans)
        return
    pre_num = -1
    for i in range(x, n):
        if pre_num != arr[i]:
            pre_num = arr[i]
            ans.append(arr[i])
            dfs(i)
            ans.pop()


n, m = map(int, sys.stdin.readline().strip().split())
arr = sorted(list(map(int, sys.stdin.readline().strip().split())))
ans = []
dfs(0)
