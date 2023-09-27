import sys

H, V = map(int, sys.stdin.readline().strip().split())
N = int(sys.stdin.readline().strip())
D = []
R = 2*H + 2*V
for i in range(N+1):
    temp = 0
    news, d = map(int, sys.stdin.readline().strip().split())
    if news == 1:
        temp += H + V + H - d
    if news == 2:
        temp += d
    if news == 3:
        temp += H + V + H + d
    if news == 4:
        temp += H + V - d
    D.append(temp)
# print(store)
# print(guard)
guard = D.pop()
ans = 0
for store in D:
    CCW = abs(store - guard)
    CW = R - CCW
    ans += min(CCW, CW)
print(ans)
