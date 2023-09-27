import sys

while True:
    a = list(input())
    if (len(a) == 1) & (a[0] == '0'):
        break
    l, r = 0, len(a) - 1
    while True:
        if l >= r:
            print('yes')
            break
        if a[l] != a[r]:
            print('no')
            break
        l += 1
        r -= 1
