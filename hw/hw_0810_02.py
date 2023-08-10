def dfs(v, p):
    visited[v] = visited[p] + 1 if ~p else 1
    for w in graph[v]:
        if w == v:
            continue
        if not visited[w]:
            dfs(w, v)


for tc in range(1, int(input())+1):
    V, E = map(int, input().split())
    segments = [tuple(map(int, input().split())) for _ in range(E)]
    S, G = map(int, input().split())
    graph = [[] for _ in range(V+1)]
    visited = [0]*(V+1)
    for i in segments:
        a, b = i
        graph[a].append(b)
    # print(graph)
    dfs(S, -1)
    # print(visited)
    if visited[G]:
        print(f'#{tc} 1')
    else:
        print(f'#{tc} 0')

'''
3
6 5
1 4
1 3
2 3
2 5
4 6
1 6
7 4
1 6
2 3
2 6
3 5
2 5
9 9
2 6
4 7
5 7
1 5
2 9
3 9
4 8
5 3
7 8
1 9

1
5 5
1 2
2 3
3 1
5 4
4 5
5 5
'''