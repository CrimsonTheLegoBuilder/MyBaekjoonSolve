import sys
sys.stdin = open('input_08_10.txt', 'r')


def dfs(v, p):
    visited[v] = visited[p] + 1 if ~p else 1
    for w in graph[v]:
        if w == v:
            continue
        if not visited[w]:
            dfs(w, v)


for tc in range(1, 11):
    _, N = map(int, input().split())
    segments = tuple(map(int, input().split()))
    graph = [[] for _ in range(100)]
    visited = [0]*100
    for i in range(0, 2*N, 2):
        a, b = segments[i], segments[i+1]
        graph[a].append(b)
    # print(graph)
    dfs(0, -1)
    # print(visited)
    if visited[99]:
        print(f'#{tc} 1')
    else:
        print(f'#{tc} 0')
