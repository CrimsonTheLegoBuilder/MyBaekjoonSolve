import sys
from itertools import permutations


def game(lst):
    score = 0
    attack = 5
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
                home = base & ~7
                while home:
                    score += 1
                    home -= home & -home
                base &= 7
            attack += 1
            attack %= 9
    return score


N = int(sys.stdin.readline().strip())
hit = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
ans = 0
for case in permutations((1, 2, 3, 4, 5, 6, 7, 8), 8):
    temp = game(list(case) + [0])
    if ans < temp:
        ans = temp
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
