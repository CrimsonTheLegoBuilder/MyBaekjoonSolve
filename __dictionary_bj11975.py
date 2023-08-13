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
dic_v = {}
dic_e = {}
N = int(sys.stdin.readline().strip())
order = str(sys.stdin.readline().strip())
for i in range(N):
    a = dic_v.setdefault((x, y), i)
    vertex(order[i])
    b = dic_v.setdefault((x, y), i+1)
    p = min(a, b)
    w = max(a, b)
    dic_e[(p, w)] = 0
V = len(dic_v)
E = len(dic_e)
# print(dic_v)
# print(dic_e)
ans = 1 + E - V
# print(E, V)
print(ans)
