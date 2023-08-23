def post(n):
    if n <= N:
        l = post(n*2)
        r = post(n*2+1)
        return l + r + tree[n]
    return 0


for tc in range(1, int(input())+1):
    N, leaf, L = map(int, input().split())
    tree = [0] * (N+1)
    for _ in range(leaf):
        V, v = map(int, input().split())
        tree[V] = v
    ans = post(L)
    print(f'#{tc} {ans}')
