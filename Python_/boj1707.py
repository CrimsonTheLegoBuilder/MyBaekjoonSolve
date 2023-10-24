import sys


def dfs(v, x):
    vtd[v] = x
    f1 = 1
    f2 = 1
    for w in G[v]:
        if vtd[w] == x:
            f1 = 0
        if not ~vtd[w]:
            f2 = dfs(w, x ^ 1)
    return f1 and f2


N = int(sys.stdin.readline().strip())
for _ in range(N):
    V, E = map(int, sys.stdin.readline().strip().split())
    G = [[] for _ in range(V+1)]
    for _ in range(E):
        v1, v2 = map(int, sys.stdin.readline().strip().split())
        G[v1].append(v2)
        G[v2].append(v1)
    vtd = [-1] * (V + 1)
    print("YES" if dfs(1, 1) else "NO")
