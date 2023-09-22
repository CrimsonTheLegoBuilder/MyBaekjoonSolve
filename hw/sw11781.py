def kruskal():
    edge.sort(key=lambda x: x[2])
    cnt = 0
    total = 0
    for s, e, w in edge:
        if find(s) != find(e):
            total += w
            cnt += 1
            union(s, e)
        if cnt == V:
            break
    return total


def find(x):
    if P[x] == x:
        return x
    P[x] = find(P[x])
    return P[x]


def union(x, y):
    x = find(x)
    y = find(y)
    if x == y:
        return 0
    if x < y:
        P[y] = x
    else:
        P[x] = y


for tc in range(int(input())):
    V, E = map(int, input().split())
    edge = [tuple(map(int, input().split())) for _ in range(E)]
    P = list(range(V+1))
    print(f"#{tc+1} {kruskal()}")
