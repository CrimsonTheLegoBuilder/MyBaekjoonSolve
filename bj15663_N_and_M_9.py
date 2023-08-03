import sys
sys.setrecursionlimit(10000)


def dfs():
    if len(ans) == m:
        temp = tuple(arr[i] for i in ans)
        if temp not in total:
            total.append(temp)
        return
    for i in range(n):
        if i in ans:
            continue
        ans.append(i)
        dfs()
        ans.pop()
    return


n, m = map(int, sys.stdin.readline().strip().split())
arr = list(map(int, sys.stdin.readline().strip().split()))
arr.sort()
ans = []
total = []
dfs()
# print(total)
for x in total:
    print(*x)
