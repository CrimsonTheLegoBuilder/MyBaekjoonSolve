import sys


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return int(((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5)


def monotone_chain(arr):  # get hull / sorting hull CCW
    arr.sort(key=lambda x: (x[0], x[1]))
    if len(arr) <= 2:
        return arr
    lower = []
    for dl in arr:
        while len(lower) > 1 and cross(lower[-2], lower[-1], lower[-1], dl) <= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(arr):
        while len(upper) > 1 and cross(upper[-2], upper[-1], upper[-1], du) <= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


H, V = map(int, sys.stdin.readline().strip().split())
hull = ((0, 0), (H, 0), (H, V), (0, V))
R = 2*H + 2*V
N = int(sys.stdin.readline().strip())
store = []
D = ()
for i in range(N+1):
    news, d = map(int, sys.stdin.readline().strip().split())
    if news == 1:
        D = (d, V, 1)
    if news == 2:
        D = (d, 0, 2)
    if news == 3:
        D = (0, V-d, 3)
    if news == 4:
        D = (H, V-d, 4)
    store.append(D)
G = store.pop()

ans = 0
for S in store:
    temp = [G]
    for j in hull:
        if cross(G, S, S, j) > 0:
            temp.append(j)
    temp.append(S)
    temp = monotone_chain(temp)
    # print(temp)
    if S[-1] == G[-1]:
        ans += abs(S[0] - G[0]) + abs(S[1] - G[1])
    else:
        CCW = 0
        for i in range(len(temp)):
            d = cal_dist(temp[i-1], temp[i])
            CCW += d
        CCW -= cal_dist(S, G)
        CW = R - CCW
        ans += min(CCW, CW)
print(ans)
