import sys
from collections import deque
INPUT = sys.stdin.readline

G = [[0] * 101 for _ in range(101)]
Q = deque()
V = [0] * 101


def bfs(p: int, n: int) -> int:
    Q.append(p)
    cnt = 0
    while Q:
        p = Q.popleft()
        for w in range(n + 1):
            if G[p][w] and not V[w]:
                Q.append(w)
                V[w] = 1
    for i in range(n + 1):
        cnt += V[i]
    return cnt - 1


if __name__ == '__main__':  # boj2606
    N = int(INPUT())
    M = int(INPUT())
    for _ in range(M):
        u, v = map(int, INPUT().split())
        G[u][v] = G[v][u] = 1
    # print(bfs(1, N))
