import sys


def dfs(x):
    global ans, v
    if x == 9:
        # print(perm)
        score = game(perm)
        ans = max(ans, score)
        return
    if x == 3:
        dfs(x+1)
    else:
        for i in range(2, 10):
            if not v & 1 << i:
                perm[x] = i-1
                v |= 1 << i
                dfs(x+1)
                v ^= 1 << i


def game(lst):
    score = 0
    attack = 0
    for i in range(N):
        base = 0
        out = 0
        while out < 3:
            cur = lst[attack]
            if not hit[i][cur]:
                out += 1
            else:
                base <<= hit[i][cur]
                base |= 1 << (hit[i][cur]-1)
                for s in range(3, 8):
                    score += (base & 1 << s) >> s
                base &= 7
            attack += 1
            attack %= 9
    return score


N = int(sys.stdin.readline().strip())
hit = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
perm = [0] * 9
v = 1
ans = 0
dfs(0)
print(ans)

'''
45
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
4 4 4 4 4 4 4 4 0
'''
