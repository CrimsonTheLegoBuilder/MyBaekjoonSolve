def merge_sort(a):
    if len(a) <= 1:
        return a
    m = len(a)//2
    l = merge_sort(a[:m])
    r = merge_sort(a[m:])
    return merge(l, r)


def merge(l, r):
    global cnt
    if l[-1] > r[-1]:
        cnt += 1
    s = []
    i, j = 0, 0
    while i < len(l) and j < len(r):
        if l[i] < r[j]:
            s.append(l[i])
            i += 1
        else:
            s.append(r[j])
            j += 1
    while i < len(l):
        s.append(l[i])
        i += 1
    while j < len(r):
        s.append(r[j])
        j += 1
    return s


for t in range(int(input())):
    cnt = 0
    L = int(input())
    a1 = list(map(int, input().split()))
    a2 = merge_sort(a1)
    print(f"#{t+1} {a2[L//2]} {cnt}")
