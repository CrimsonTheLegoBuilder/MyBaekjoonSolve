import sys
sys.setrecursionlimit(10000)


def dfs():
    if len(ans) == m:
        print(*ans)
        return
    pre_num = -1
    for i in range(n):
        if visited[i] == 0 and pre_num != arr[i]:
            pre_num = arr[i]
            visited[i] = 1
            ans.append(arr[i])
            dfs()
            visited[i] = 0
            ans.pop()


n, m = map(int, sys.stdin.readline().strip().split())
arr = sorted(list(map(int, sys.stdin.readline().strip().split())))
visited = [0] * n
ans = []
dfs()
