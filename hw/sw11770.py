def q_sort(a, l, r):
    if l < r:
        p = a[l]
        i, j = l, r
        while i <= j:
            while i <= j and a[i] <= p:
                i += 1
            while i <= j and a[j] >= p:
                j -= 1
            if i < j:
                a[i], a[j] = a[j], a[i]
        a[l], a[j] = a[j], a[l]
        q_sort(a, l, j-1)
        q_sort(a, j+1, r)


for t in range(int(input())):
    N = int(input())
    a1 = list(map(int, input().split()))
    q_sort(a1, 0, len(a1)-1)
    print(f"#{t+1} {a1[N//2]}")
