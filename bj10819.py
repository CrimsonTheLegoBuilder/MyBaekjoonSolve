import sys


def dfs():
    global ans
    if len(temp) == N:
        total = 0
        for i in range(N-1):
            total += abs(arr[temp[i]] - arr[temp[i+1]])
        ans = max(ans, total)
        return
    for x in range(N):
        if x not in temp:
            temp.append(x)
            dfs()
            temp.pop()


N = int(sys.stdin.readline().strip())
arr = tuple(map(int, sys.stdin.readline().strip().split()))
temp = []
ans = 0
dfs()
print(ans)
