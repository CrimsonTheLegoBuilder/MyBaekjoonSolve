from collections import deque


def bfs(b):
    V[b] += 1
    Q.append(b)
    while Q:
        p = Q.popleft()
        if p == M:
            return V[p]
        if 0 < p << 1 <= 1000000 and not ~V[p << 1]:
            Q.append(p << 1)
            V[p << 1] = V[p] + 1
        if 0 < p+1 <= 1000000 and not ~V[p+1]:
            Q.append(p+1)
            V[p+1] = V[p] + 1
        if 0 < p-1 <= 1000000 and not ~V[p-1]:
            Q.append(p-1)
            V[p-1] = V[p] + 1
        if 0 < p-10 <= 1000000 and not ~V[p-10]:
            Q.append(p-10)
            V[p-10] = V[p] + 1


for tc in range(int(input())):
    N, M = map(int, input().split())
    V = [-1] * 1000001
    Q = deque()
    A = []
    ans = bfs(N)
    print(f"#{tc+1} {ans}")
