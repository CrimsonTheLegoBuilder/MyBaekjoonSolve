import sys

n = int(sys.stdin.readline().strip())

dots = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
dic_x = {}
dic_y = {}
cnt = 0
for x, y in dots:
    if x in dic_x:
        dic_x[x] += 1
    else:
        dic_x[x] = 1
    if y in dic_y:
        dic_y[y] += 1
    else:
        dic_y[y] = 1
# print(dic_x)
# print(dic_y)

for x, y in dots:
    cnt += (dic_x[x] - 1) * (dic_y[y] - 1)

print(cnt)
