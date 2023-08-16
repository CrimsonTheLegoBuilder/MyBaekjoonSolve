def dfs(x, s):
    global total
    if s > total:
        return
    if x == N:
        if total > s:
            total = s
        return
    for i in range(N):
        if bit[i]:
            continue
        bit[i] = 1
        dfs(x+1, s+arr[x][i])
        bit[i] = 0


for tc in range(1, int(input())+1):
    N = int(input())
    arr = [tuple(map(int, input().split())) for _ in range(N)]
    bit = [0]*N
    total = int(12e12)
    dfs(0, 0)
    print(f'#{tc} {total}')
