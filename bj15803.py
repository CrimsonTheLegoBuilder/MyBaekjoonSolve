import sys


def cross(x1, y1, x2, y2, x3, y3):
    ans = ((x1 - x2) * (y2 - y3)) - ((y1 - y2) * (x2 - x3))
    if ans == 0:
        print('WHERE IS MY CHICKEN?')
    else:
        print('WINNER WINNER CHICKEN DINNER!')


x1, y1 = map(int, sys.stdin.readline().strip().split())
x2, y2 = map(int, sys.stdin.readline().strip().split())
x3, y3 = map(int, sys.stdin.readline().strip().split())
cross(x1, y1, x2, y2, x3, y3)
