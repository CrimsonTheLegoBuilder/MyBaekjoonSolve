import sys

n = int(sys.stdin.readline().strip())
team = [False for _ in range(n)]
stats = []
for _ in range(n):
    stats.append(list(map(int, sys.stdin.readline().strip().split())))
min_ = 1e9


def diff_team(depth, idx):
    global min_
    if depth == n//2:
        T = 0
        F = 0
        for i in range(n):
            for j in range(n):
                if team[i] and team[j]:
                    T += stats[i][j]
                elif not team[i] and not team[j]:
                    F += stats[i][j]
        min_ = min(min_, abs(T - F))
        return
    for k in range(idx, n):
        if not team[k]:
            team[k] = True
            diff_team(depth + 1, k + 1)
            team[k] = False


diff_team(0, 0)
print(min_)
