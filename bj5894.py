import sys


def dfs():
    global path
    if len(comb) == n:
        if cows[comb[0]][0] != 0 and cows[comb[0]][1] != 0:
            return
        if cows[comb[-1]][0] != 0 and cows[comb[-1]][1] != 0:
            return
        for i in range(n - 1):
            if cows[comb[i]][0] != cows[comb[i + 1]][0] and cows[comb[i]][1] != cows[comb[i + 1]][1]:
                return
        # print(comb)
        # print(cows[comb[0]][0], cows[comb[0]][1], cows[comb[-1]][0], cows[comb[-1]][1])
        path += 1
        return
    for i in range(n):
        if i in comb:
            continue
        comb.append(i)
        dfs()
        comb.pop()


n = int(sys.stdin.readline().strip())
cows = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
path = 0
comb = []
dfs()
print(path)
