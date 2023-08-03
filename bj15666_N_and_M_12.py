import sys
sys.setrecursionlimit(10000)


def dfs(x):
    if len(ans) == m:
        print(*ans)
        return
    # visited = [0] * 10001
    pre_num = -1
    for i in range(x, n):
        if pre_num != arr[i]:  # not visited[arr[i]]:  # and
            pre_num = arr[i]
            # visited[arr[i]] = 1
            ans.append(arr[i])
            dfs(i)
            # visited[arr[i]] = 0
            ans.pop()


n, m = map(int, sys.stdin.readline().strip().split())
arr = sorted(list(map(int, sys.stdin.readline().strip().split())))
ans = []
dfs(0)
