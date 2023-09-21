from heapq import heappush as h_push
from heapq import heappop as h_pop
import sys


def find(x):
    if P[x] == x:
        return x
    P[x] = find(P[x])
    return P[x]


def union(x, y):
    x = find(x)
    y = find(y)
    if x == y:
        return
    if x < y:
        P[y] = x
    else:
        P[x] = y


def kruskal(s):
    return


V, E = map(int, sys.stdin.readline().strip().split())
# graph = [[0]*V for _ in range(E)]
edge = []
total = 0
for _ in range(E):
    f, t, W = map(int, sys.stdin.readline().strip().split())
    edge.append((W, f, t))

edge.sort()
P = [i for i in range(V)]

cnt = 0
for W, f, t in edge:
    if find(f) != find(t):
        cnt += 1
        total += W
        union(f, t)
        if cnt == V:
            break

print(total)


'''
7 11
0 1 32
0 2 31
0 5 60
0 6 51
1 2 21
2 4 46
2 6 25
3 4 34
3 5 18
4 5 40
4 6 51
'''