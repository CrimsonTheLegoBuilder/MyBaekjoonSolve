import sys


def dfs(o):
    global ans
    if len(combi) == 2:
        y_s = sorted([d[1] for d in dots if combi[0] <= d[0] <= combi[1]])
        if len(y_s) < n//2:
            return
        for k in range(len(y_s) - n//2 + 1):
            ans = min(ans, (combi[1] - combi[0] + 2) * (y_s[k + n//2 - 1] - y_s[k] + 2))
            # print(ans)
        return
    for o1 in range(o, len(X)):
        combi.append(X[o1])
        dfs(o1)
        combi.pop()


n = int(sys.stdin.readline().strip())
X_ = []
X = []
combi = []
dots = []
ans = int(9e12)
for _ in range(n):
    x, y = map(int, sys.stdin.readline().strip().split())
    dots.append((x, y))
    X_.append(x)

X_.sort()
X.append(X_[0])
for i in range(1, n):
    if X_[i] != X_[i - 1]:
        X.append(X_[i])

dfs(0)
print(ans)
