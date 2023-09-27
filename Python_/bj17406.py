import sys


def arr_rotate(temp, r, c, s):
    r -= 1
    c -= 1
    for d in range(1, s+1):
        for i in range(r-d, r+d):
            temp[i][c-d], temp[i+1][c-d] = temp[i+1][c-d], temp[i][c-d]
        for j in range(c-d, c+d):
            temp[r+d][j], temp[r+d][j+1] = temp[r+d][j+1], temp[r+d][j]
        for i in range(r+d, r-d, -1):
            temp[i][c+d], temp[i-1][c+d] = temp[i-1][c+d], temp[i][c+d]
        for j in range(c+d, c-d+1, -1):
            temp[r-d][j], temp[r-d][j-1] = temp[r-d][j-1], temp[r-d][j]


def bt(arr, x):
    global ans
    if x == K:
        temp = [row[:] for row in arr]
        for i in range(K):
            r, c, s = order[perm[i]]
            arr_rotate(temp, r, c, s)
        # for row in temp:
        #     print(*row)
        # print()
        for row in temp:
            total = sum(row)
            ans = min(ans, total)
        return
    for i in range(K):
        if visited[i]:
            continue
        perm[x] = i
        visited[i] = 1
        bt(arr, x+1)
        visited[i] = 0


N, M, K = map(int, sys.stdin.readline().strip().split())
plate = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
order = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(K)]
visited = [0] * K
perm = [0] * K
ans = int(12e12)
bt(plate, 0)
print(ans)
