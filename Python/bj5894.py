import sys
sys.setrecursionlimit(10000)


def cross(d1, d2, d3):  # cross product / CCW / get area / get torque /for 3 dots
    return (d2[0] - d1[0]) * (d3[1] - d2[1]) - (d2[1] - d1[1]) * (d3[0] - d2[0])


def dots(d1, d2, d3):  # dots product / get projection / get force / for 3 dots
    return (d2[0] - d1[0]) * (d3[0] - d2[0]) + (d2[1] - d1[1]) * (d3[1] - d2[1])


def dfs():
    global path
    if len(comb) == n + 1:
        # print(comb)
        if dots(cows[comb[-2]], cows[comb[-1]], (0, 0)) == 0 or \
                cross(cows[comb[-2]], cows[comb[-1]], (0, 0)) == 0 and dots(cows[comb[-2]], cows[comb[-1]], (0, 0)) < 0:
            path += 1
        return
    for i in range(1, n + 1):
        if i in comb:
            continue
        if len(comb) == 1 and (cows[i][0] == 0 or cows[i][1] == 0):
            comb.append(i)
            # print(comb)
            dfs()
            comb.pop()
        if len(comb) > 1 and (dots(cows[comb[-2]], cows[comb[-1]], cows[i]) == 0 or
                              (cross(cows[comb[-2]], cows[comb[-1]], cows[i]) == 0 and
                              dots(cows[comb[-2]], cows[comb[-1]], cows[i]) < 0)):
            # print(i, dots(cows[comb[-2]], cows[comb[-1]], cows[i]))
            comb.append(i)
            # print(comb)
            dfs()
            comb.pop()
    return


n = int(sys.stdin.readline().strip())
cows = [(0, 0)] + [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
# print(cows)
path = 0
comb = [0]
dfs()
print(path)
