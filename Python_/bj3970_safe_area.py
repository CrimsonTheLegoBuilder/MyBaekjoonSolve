import sys
from functools import cmp_to_key


def comp(c1, c2):
    if c1[1] - c1[2] == c2[1] - c2[2]:
        if c1[0] == c2[0]:
            if c1[2] < c2[2]:
                return -1
            elif c1[2] == c2[2]:
                return 0
            else:
                return 1
        elif c1[0] < c2[0]:
            return -1
        elif c1[0] < c2[0]:
            return 1
    elif c1[1] - c1[2] < c2[1] - c2[2]:
        return -1
    elif c1[1] - c1[2] > c2[1] - c2[2]:
        return 1


tc = int(sys.stdin.readline().strip())
for _ in range(tc):
    N = int(sys.stdin.readline().strip())
    circles = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
    circles.sort(key=cmp_to_key(comp))
    print(circles)

'''
4
3
2 2 1
8 2 1
5 6 1
2
6 4 2
2 4 1
4
2 2 2
6 1 1
5 5 2
1 6 1
4
6 1 1
5 5 2
2 2 2
1 6 1
'''