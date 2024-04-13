from collections import deque
import sys
input = sys.stdin.readline
dx, dy = [0, 0, 1, -1], [1, -1, 0, 0]

def dfs(q, visited, goal):
    while q:
        x, y, cnt = q.popleft()
        for i in range(4):
            nx, ny = x + dx[i], y + dy[i]
            if 0 <= nx < r and 0 <= ny < c and not visited[nx][ny] and (graph[nx][ny] == '.' or graph[nx][ny] == 'J'):
                visited[nx][ny] = 1
                q.append([nx, ny, cnt + 1])
                if nx == r - 1 or ny == c - 1 or nx == 0 or ny == 0:
                    goal.append([cnt + 1, nx, ny])
    return -1

r, c = map(int, input().rstrip().split())
graph = list(list(input().rstrip()) for _ in range(r))
result = 0

fvisited = list(([0] * c) for _ in range(r))
fq = deque()
fgoal = list() #각 가장자리 지점들에 도달하는 경우

hvisited = list(([0] * c) for _ in range(r))
hq = deque()
hgoal = list()

# 큐 채우기
for i in range(r):
    for j in range(c):
        if graph[i][j] == 'J':
            hq.append([i, j, 1])
            hvisited[i][j] = 1
        elif graph[i][j] == 'F':
            fq.append([i, j, 1])

dfs(hq, hvisited, hgoal)
dfs(fq, fvisited, fgoal)

hgoal.sort()
fgoal.sort()

# 각 가장자리 도달하는 경우
for cnt, x, y in hgoal:
    for fcnt, fx, fy in fgoal:
        if fx == x and fy == y and fcnt > cnt:
            result = cnt
            break
    if result == cnt:
        break

if result:
    print(result)
else:
    print("IMPOSSIBLE")

'''

5 5
.....
####.
..J#F
####.
.....
'''
