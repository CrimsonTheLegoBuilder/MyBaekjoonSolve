import sys


H, V = map(int, sys.stdin.readline().strip().split())
R = 2*H + 2*V
N = int(sys.stdin.readline().strip())
store = []
guard = ()
D = ()
for i in range(N+1):
    news, d = map(int, sys.stdin.readline().strip().split())
    if news == 1:
        D = (d, V, 1, 'N')
    if news == 2:
        D = (d, 0, 1, 'S')
    if news == 3:
        D = (0, V-d, 0, 'W')
    if news == 4:
        D = (H, V-d, 0, 'E')
    store.append(D)
# print(store)
# print(guard)
xg, yg, ng, sg = store.pop()
ans = 0
for i in store:
    x, y, n, s = i
    # print(x, y, n, s)
    # print(xg, yg, ng, sg)
    # print(ans)
    if s == sg:
        if n:
            ans += abs(xg - x)
        else:
            ans += abs(yg - y)
    elif s != sg:
        if n == ng:
            if n == 1:
                CCW = V + x + xg
                ans += min(CCW, R-CCW)
            elif not n:
                CCW = H + y + yg
                ans += min(CCW, R-CCW)
        elif n != ng:  # and s != sg:
            if s == 'N':
                if sg == 'E':
                    ans += V + H - x - yg
                elif sg == 'W':
                    ans += V + x - yg
            elif s == 'E':
                if sg == 'N':
                    ans += V + H - y - xg
                elif sg == 'S':
                    ans += V - y + xg
            elif s == 'W':
                if sg == 'N':
                    ans += V - y + xg
                elif sg == 'S':
                    ans += y + xg
            elif s == 'S':
                if sg == 'E':
                    ans += H - x + yg
                elif sg == 'W':
                    ans += x + yg
print(ans)
