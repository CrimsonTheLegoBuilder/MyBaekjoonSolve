def bi_search(arr, t):
    d = 0
    s, e = 0, len(arr)-1
    while s <= e:
        m = (s + e) // 2
        if arr[m] == t:
            return 1
        elif arr[m] < t:
            if d == 1:
                return 0
            s = m+1
            d = 1
        else:
            if d == -1:
                return 0
            e = m-1
            d = -1
    return 0


for tc in range(int(input())):
    N, M = map(int, input().split())
    A = sorted(list(map(int, input().split())))
    B = list(map(int, input().split()))
    ans = 0
    for x in B:
        ans += bi_search(A, x)
    print(f"#{tc+1} {ans}")
