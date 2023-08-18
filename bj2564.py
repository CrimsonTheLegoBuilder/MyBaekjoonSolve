import sys


H, V = map(int, sys.stdin.readline().strip().split())
hull = ((0, 0), (H, 0), (H, V), (0, V))
N = int(sys.stdin.readline().strip())
store = []
guard = ()
D = ()
for i in range(N+1):
    news, d = map(int, sys.stdin.readline().strip().split())
    if news == 1:
        D = (d, V, 1, 'N')
    elif news == 2:
        D = (d, 0, 1, 'S')
    elif news == 3:
        D = (0, V-d, 0, 'W')
    elif news == 4:
        D = (H, V-d, 0, 'E')
    if i == N:
        guard = D
    else:
        store.append(D)
print(store)
print(guard)
xg, yg, ng, sg = guard
ans = 0
for x, y, n, s in store:
    print(ans)
    if n == ng and sg == s:
        ans += abs(xg - x) + abs(yg - y)
    elif n == ng and sg != s:
        if n:
            ans += V + min(x + xg, H * 2 - x - xg)
        else:
            ans += H + min(y + yg, V * 2 - y - yg)
    elif n != ng:
        if s == 'N':
            if sg == 'E':
                ans += V + H - x - yg
            if sg == 'W':
                ans += V + x - yg
        if s == 'E':
            if sg == 'N':
                ans += V + H - y - xg
            if sg == 'S':
                ans += V - y + xg
        if s == 'W':
            if sg == 'N':
                ans += V - y + xg
            if sg == 'S':
                ans += y + xg
        if s == 'S':
            if sg == 'E':
                ans += H - x + yg
            if sg == 'W':
                ans += x + yg


print(ans)
