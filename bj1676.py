import sys
n = int(sys.stdin.readline().strip())
c = n // 125
rs = n % 125
s = rs // 25
r5 = rs % 25
a = r5 // 5
print(31 * c + 6*s + a)


def facto(x):
    if x <= 1:
        return 1
    return x * facto(x-1)


# print(str(facto(n))[-(31 * c + 6*s + a) - 1:])
