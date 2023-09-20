def find_set(x):
    if P[x] == x:
        return x
    P[x] = find_set(P[x])
    return P[x]


def union(x, y):
    x = find_set(x)
    y = find_set(y)
    if x == y:
        return
    if x < y:
        P[y] = x
    else:
        P[x] = y


for tc in range(int(input())):
    N, M = map(int, input().split())
    A = tuple(map(int, input().split()))
    P = [i for i in range(N)]
    for i in range(M):
        j, k = A[i << 1], A[(i << 1) + 1]
        union(j-1, k-1)
    for i in range(N):
        find_set(i)
    # print(P)
    print(f"#{tc+1} {len(set(P))}")
