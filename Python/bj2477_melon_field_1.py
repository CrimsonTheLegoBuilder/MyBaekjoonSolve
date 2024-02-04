import sys
import collections as cl

n = int(sys.stdin.readline().strip())
area = cl.deque()
dic = {}
for _ in range(6):
    a, b = map(int, sys.stdin.readline().strip().split())
    area.append([a, b])
    if a in dic:
        dic[a] += 1
    else:
        dic[a] = 1

for _ in range(6):
    area.rotate(1)
    if (dic[area[0][0]] == 1) & (dic[area[1][0]] == 1):
        break

print(n * (area[0][1] * area[1][1] - area[3][1] * area[4][1]))
