import sys; sys.stdin = open("sw1249_input.txt", "r")
from heapq import heappop as h_pop
from heapq import heappush as h_push
MAX = 10001


def dijkstra(r, c):
    h_push(H, (0, r, c))
    cost[r][c] = 0
    while H:
        c, i, j = h_pop(H)
        if cost[i][j] < c:
            continue
        # if i == N-1 and j == N-1:
        #     return
        for k in range(4):
            ni = i + drc[k][0]
            nj = j + drc[k][1]
            if 0 <= ni < N and 0 <= nj < N:
                c_ = graph[ni][nj]
                if cost[ni][nj] > c + c_:
                    cost[ni][nj] = c + c_
                    h_push(H, (c + c_, ni, nj))


for tc in range(int(input())):
    N = int(input())
    graph = [tuple(map(int, list(input()))) for _ in range(N)]
    H = []
    cost = [[MAX]*N for _ in range(N)]
    drc = ((1, 0), (0, 1), (-1, 0), (0, -1))
    dijkstra(0, 0)
    print(f"#{tc+1} {cost[N-1][N-1]}")


'''
#1 2
#2 2
#3 8
#4 57
#5 151
#6 257
#7 18
#8 160
#9 414
#10 395
'''
