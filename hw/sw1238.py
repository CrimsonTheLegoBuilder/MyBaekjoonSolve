import sys
sys.stdin = open("sw1238_input.txt", "r")
from collections import deque


def bfs(v):
    global ans
    Q.append(v)
    V[v] += 1
    m = 1
    while Q:
        p = Q.popleft()
        for w in graph[p]:
            if not ~V[w]:
                V[w] = V[p] + 1
                m = V[w]
                Q.append(w)

    for i in range(100):
        if V[i] == m:
            ans = i+1


for tc in range(1, 11):
    ans = 0
    Q = deque()
    N, S = map(int, input().split())
    link = tuple(map(int, input().split()))
    graph = [[] for _ in range(100)]
    V = [-1]*100
    for l in range(0, N-1, 2):
        graph[link[l]-1].append(link[l+1]-1)
    bfs(S-1)
    print(f"#{tc} {ans}")
