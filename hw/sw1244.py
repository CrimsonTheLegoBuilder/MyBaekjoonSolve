def swap(x, y): arr[x], arr[y] = arr[y], arr[x]


def conv_no():
    temp = 0
    for i in arr:
        temp *= 10
        temp += i
    return temp


def dfs(c):
    global ans
    state = conv_no(), c
    if visited.get(state):
        return
    visited.setdefault(state, 1)
    if not c:
        ans = max(ans, state[0])
        return
    for i in range(L-1):
        for j in range(i+1, L):
            swap(i, j)
            dfs(c-1)
            swap(i, j)


for tc in range(1, int(input())+1):
    N, M = map(int, input().split())
    arr = list(map(int, str(N)))
    L = len(arr)
    ans = 0
    visited = {}
    dfs(M)
    print(f'#{tc} {ans}')
