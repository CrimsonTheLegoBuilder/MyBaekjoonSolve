# import sys
# sys.stdin = open('input_maze.txt', 'r')
from collections import deque


def bfs(v):
    queue.append(v)
    visited[v] = 1
    while queue:
        p = queue.popleft()
        for w in range(1, V+1):
            if graph[p][w] < limit and not visited[w]:
                queue.append(w)
                visited[w] = visited[p] + 1
                # if graph[v][w] > visited[w]:
                #     graph[v][w] = visited[w]
                if w == G:
                    return visited[p]
    return 0


limit = int(12e12)
for tc in range(1, int(input())+1):
    V, E = map(int, input().split())
    graph = [[limit]*(V+1) for _ in range(V+1)]
    visited = [0]*(V+1)
    queue = deque()
    for _ in range(E):
        v1, v2 = map(int, input().split())
        graph[v1][v2] = 1
        graph[v2][v1] = 1
    # print(graph)
    S, G = map(int, input().split())
    ans = bfs(S)
    # print(visited)
    print(f'#{tc} {ans}')
