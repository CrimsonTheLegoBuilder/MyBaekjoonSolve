import sys
from math import inf


def meet(y, slp, dot):
    if slp[1] == 0:
        return
    x = slp[0] * (y - dot[1]) / slp[1] + dot[0]
    return x


a = '......as...'
print(a[:-2])
