for tc in range(1, int(input())+1):
    N = int(input())
    order = list(map(int, input().split()))
    tree = [0] * (N+1)
    for i in range(1, N+1):
        tree[i] = order[i-1]
        c, p = i, i//2
        while tree[p]:
            if tree[p] > tree[c]:
                tree[p], tree[c] = tree[c], tree[p]
            c = p
            p //= 2
    # print(tree)
    ans = 0
    while N:
        N //= 2
        ans += tree[N]
    print(f'#{tc} {ans}')
