import sys
sys.setrecursionlimit(10000)


def dfs():
    if len(ans) == m:
        print(*ans)
        return
    for i in range(n):
        ans.append(arr[i])
        dfs()
        ans.pop()


n, m = map(int, sys.stdin.readline().strip().split())
arr = sorted(list(map(int, sys.stdin.readline().strip().split())))
ans = []
dfs()
