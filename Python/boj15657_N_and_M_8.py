import sys
sys.setrecursionlimit(10000)


def dfs(x):
    if len(ans) == m:
        print(*ans)
        return
    for i in range(x, n):
        ans.append(arr[i])
        dfs(i)
        ans.pop()
    return


n, m = map(int, sys.stdin.readline().strip().split())
arr = list(map(int, sys.stdin.readline().strip().split()))
arr.sort()
ans = []
dfs(0)
