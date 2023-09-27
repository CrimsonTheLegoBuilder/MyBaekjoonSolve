import sys
sys.setrecursionlimit(10000)


def dfs(x):
    if len(ans) == m:
        print(*ans)
        return
    for i in range(x, n):
        ans.append(arr[i])
        dfs(i+1)
        ans.pop()


n, m = map(int, sys.stdin.readline().strip().split())
arr = sorted(list(map(int, sys.stdin.readline().strip().split())))
ans = []
dfs(0)
