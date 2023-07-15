n, k = map(int, input().split())
A = [*map(int, input().split())]

d = {1: 0}


def f(n):
    if n not in d:
        d[n] = n+(d[(n+1)//2] if (n+1)//2 in d else f((n+1)//2)) + (d[n//2] if n//2 in d else f(n//2))
    return d[n]


f(n)
if k > d[n]:
    print(-1)
    exit()


def find(a, b, k):
    m = b+1-a
    if k > d[m]-m:
        print(sorted(A[a:b+1])[k+m-d[m]-1])
        exit()
    if k <= d[(m+1)//2]:
        find(a, a+(m-1)//2, k)
    else:
        find(a+(m+1)//2, b, k-d[(m+1)//2])


find(0, n-1, k)
