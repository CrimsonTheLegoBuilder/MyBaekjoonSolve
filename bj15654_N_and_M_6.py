import sys
sys.setrecursionlimit(10000)


def dfs():
    if len(ans) == m:
        print(*ans)
        return
    for i in range(n):
        if arr[i] in ans:
            continue
        ans.append(arr[i])
        dfs()
        ans.pop()


n, m = map(int, sys.stdin.readline().strip().split())
arr = list(map(int, sys.stdin.readline().strip().split()))
arr.sort()
ans = []
dfs()
