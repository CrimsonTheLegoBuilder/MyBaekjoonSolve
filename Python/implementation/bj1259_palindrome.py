import sys

while True:
    a = list(input())
    if (len(a) == 1) & (a[0] == '0'):
        break
    l, r = 0, len(a) - 1
    flag = 1
    while l <= r:
        if a[l] != a[r]:
            flag = 0
            print('no')
            break
        l += 1
        r -= 1
    if flag:
        print('yes')
