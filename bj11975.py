import sys


def vertex(o):
    global x, y
    if o == 'N':
        y += 1
    if o == 'E':
        x += 1
    if o == 'W':
        x -= 1
    if o == 'S':
        y -= 1


x, y = 0, 0
dic_v = {(0, 0): 0}
dic_e = {}
N = int(sys.stdin.readline().strip())
order = str(sys.stdin.readline().strip())
for i in range(N):
    xp, yp = x, y
    vertex(order[i])
    if dic_v.get((x, y)):
        continue
    else:
        dic_v[(x, y)] = i + 1
    p = min(dic_v[(xp, yp)], dic_v[(x, y)])
    w = max(dic_v[(xp, yp)], dic_v[(x, y)])
    dic_e[(p, w)] = 0
V = len(dic_v)
E = len(dic_e)
print(dic_v)
print(dic_e)
ans = 1 + E - V
print(E, V)
print(ans)
