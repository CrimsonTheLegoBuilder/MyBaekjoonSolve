from collections import deque
# 부분집합을 구하고 그 부분집합들이 모두 잘 연결이 되어있는지를 판단하면 됨


def connected(group):
    Q = deque()
    Q.append(group[0])
    visited[group[0]] = 1

    while Q:
        v = Q.popleft()
        for w in adj[v]:
            if w in group and not visited[w]:
                Q.append(w)
                visited[w] = 1

    # 다 방문했는지 check
    # print(group, visited)
    for g in group:
        if visited[g] == 0:
            return 0

    return 1


N = int(input())
people = [0] + list(map(int, input().strip().split()))

# 인접 리스트 만들기
adj = [[] for _ in range(N + 1)]
for i in range(1, N + 1):
    adj[i].extend(list(map(int, input().strip().split())))

# for row in adj:
#     print(*row)
min_v = 999999
# 비트마스킹을 이용한 부분집합 생성
for i in range(1, 1 << (N-1)):
    group1 = []
    group2 = []
    for j in range(N):
        if i & (1 << j):
            group1.append(j + 1)
        else:
            group2.append(j + 1)
    # print(group1, group2)

    visited = [0] * (N + 1)  # 방문체크 위치주의 (매번 초기화 해줘야 됨)
    # 두 그룹이 정상적으로 연결이 되어있다면? 그룹원의 합을 구하고 두 그룹의 최소값 찾기
    if connected(group1) and connected(group2):
        group1_value = sum([people[g1] for g1 in group1])
        group2_value = sum([people[g2] for g2 in group2])
        # print(group1_value, group2_value)
        min_v = min(min_v, abs(group1_value - group2_value))

if min_v == 999999:
    min_v = -1

print(min_v)



