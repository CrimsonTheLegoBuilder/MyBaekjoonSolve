def partition(l, r):
    if l < r:
        m = (l+r)//2
        if m - l > 1 and not (r - l) % 2:
            m -= 1
        partition(l, m)
        partition(m+1, r)
        merge(l, r)


def merge(l, r):
    global cnt
    m = (l+r)//2
    if m - l > 1 and not (r - l) % 2:
        m -= 1
    if a1[m] > a1[r]:
        cnt += 1
    i, j, s = l, m+1, l
    while i <= m and j <= r:
        if a1[i] < a1[j]:
            a2[s] = a1[i]
            i += 1
            s += 1
        else:
            a2[s] = a1[j]
            j += 1
            s += 1
    while i <= m:
        a2[s] = a1[i]
        i += 1
        s += 1
    while j <= r:
        a2[s] = a1[j]
        j += 1
        s += 1
    for k in range(l, r+1):
        a1[k] = a2[k]
    # print(a2)


for t in range(int(input())):
    cnt = 0
    L = int(input())
    a1 = list(map(int, input().split()))
    a2 = [0]*L
    partition(0, L)
    # print(a1)
    print(f"#{t+1} {a1[L//2]} {cnt-1}")
