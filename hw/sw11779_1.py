from collections import deque
MAX = 1000000


def m(x):
    return x+1, x-1, x << 1, x-10


def bfs(b):
    V[b] += 1
    Q.append(b)
    while Q:
        p = Q.popleft()
        if p == M:
            return V[p]
        for i in m(p):
            if 0 < i <= MAX and not ~V[i]:
                V[i] = V[p] + 1
                Q.append(i)


for tc in range(int(input())):
    N, M = map(int, input().split())
    V = [-1] * (MAX+1)
    Q = deque()
    ans = bfs(N)
    print(f"#{tc+1} {ans}")
