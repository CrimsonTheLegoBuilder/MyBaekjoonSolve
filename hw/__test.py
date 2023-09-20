# graph = [
#     [1, 3],
#     [0, 2, 3, 4],
#     [1],
#     [0, 1, 4],
#     [1, 3]
# ]
#
#
# visited = [0]*5
# stack = []
#
#
# def dfs_stack(v):
#     visited[v] = 1
#     stack.append(v)
#     while stack:
#         p = stack.pop()
#         for w in graph[p]:
#             if not visited[w]:
#                 stack.append(w)
#                 visited[w] = visited[p] + 1
#
#
# dfs_stack(0)
# print(visited)


# make set
parent = [i for i in range(10)]  # 이게 집합...?


# find
def find_set(x):
    if parent[x] == x:
        return x
    # return find_set(parent[x])     # 찾아서 반환
    parent[x] = find_set(parent[x])  # 찾아서 수정 / 경로 압축
    return parent[x]


# union find
def union(x, y):
    x = find_set(x)
    y = find_set(y)
    # 이미 같은 집합이라면?
    if x == y:
        print('cycle')
        return  # 합치지 않는다
    # 다른 집합이라면?
    if x < y:
        parent[y] = x
    else:
        parent[x] = y


union(0, 1)
union(2, 3)
union(1, 3)
union(0, 2)
print(find_set(0))
print(find_set(1))
t_x = 0
t_y = 2
if find_set(t_x) == find_set(t_y):
    print(1)
else:
    print(0)
