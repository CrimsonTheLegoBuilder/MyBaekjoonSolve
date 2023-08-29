def bt(x, s):
    global ans
    # if s > K:
    #     return
    if x == N:
        if s == K:
            ans += 1
        return
    bt(x+1, s)
    if s+arr[x] <= K:
        bt(x+1, s+arr[x])


for tc in range(1, int(input())+1):
    N, K = map(int, input().split())
    arr = list(map(int, input().split()))
    visited = [0] * N
    ans = 0
    bt(0, 0)
    print(f'#{tc} {ans}')
