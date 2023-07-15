import sys

n = int(sys.stdin.readline().strip())

stats = []
team_s = []
team_l = []
combi = []
min_ = 1e9

for _ in range(n):
    stats.append(list(map(int, sys.stdin.readline().strip().split())))


def org_team(x):
    global min_, team_l, S_stat, L_stat
    if len(team_s) == n//2:
        S_stat = 0
        L_stat = 0
        team_l.clear()
        for i in range(n):
            if i not in team_s:
                team_l.append(i)
        S_sum(team_s)
        L_sum(team_l)
        dif = abs(S_stat - L_stat)
        if dif < min_:
            min_ = dif
            return
    for j in range(x, n):
        if j not in team_s:
            team_s.append(j)
            org_team(x + 1)
            team_s.pop()


def S_sum(ans):
    global S_stat
    if len(combi) == 2:
        S_stat += stats[combi[0]][combi[1]]
    for k in range(n // 2):
        if ans[k] not in combi:
            combi.append(ans[k])
            S_sum(ans)
            combi.pop()


def L_sum(ans):
    global L_stat
    if len(combi) == 2:
        L_stat += stats[combi[0]][combi[1]]
    for k in range(n // 2):
        if ans[k] not in combi:
            combi.append(ans[k])
            L_sum(ans)
            combi.pop()


S_stat = 0
L_stat = 0
org_team(0)
print(min_)
